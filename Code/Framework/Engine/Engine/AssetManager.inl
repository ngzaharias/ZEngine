#pragma once

template<typename TAsset, typename TLoader, typename... TArgs>
void eng::AssetManager::RegisterAsset(const str::Name& type, TArgs&&... args)
{
	static_assert(std::is_convertible<TAsset*, eng::Asset*>::value, "Type must inherit from eng::Asset using the [public] keyword!");
	static_assert(std::is_base_of<eng::Asset, TAsset>::value, "Type isn't a base of eng::Asset!");

	static_assert(std::is_convertible<TLoader*, eng::AssetLoader*>::value, "Type must inherit from eng::AssetLoader using the [public] keyword!");
	static_assert(std::is_base_of<eng::AssetLoader, TLoader>::value, "Type isn't a base of eng::AssetLoader!");

	Z_PANIC(!enumerate::Contains(m_Registry, type), "Type '{}' has already been registered!", ToTypeName<TAsset>());

	constexpr TypeHash typeHash = ToTypeHash<TAsset>();
	m_TypeHashs[typeHash] = type;

	eng::AssetEntry& entry = m_Registry[type];
	entry.m_Loader = new TLoader(std::forward<TArgs>(args)...);
	entry.m_Loader->m_AssetManager = this;
	entry.m_Schedule = &ScheduleLoad<TAsset>;

	eng::AssetMethods& methods = entry.m_Methods;
	if constexpr (requires (TAsset a, TLoader l) { l.Bind(a); })
		methods.m_Bind = &BindMethod<TAsset, TLoader>;
	if constexpr (requires (TAsset a, TLoader l) { l.Unbind(a); })
		methods.m_Unbind = &UnbindMethod<TAsset, TLoader>;

	if constexpr (requires (TAsset a, TLoader l, eng::Visitor v) { l.Import(a, v); })
		methods.m_Import = &ImportMethod<TAsset, TLoader>;
	if constexpr (requires (TAsset a, TLoader l, eng::Visitor v) { l.Load(a, v); })
		methods.m_Load = &LoadMethod<TAsset, TLoader>;
	if constexpr (requires (TAsset a, TLoader l, eng::Visitor v) { l.Save(a, v); })
		methods.m_Save = &SaveMethod<TAsset, TLoader>;
	if constexpr (requires (TAsset a, TLoader l) { l.Unload(a); })
		methods.m_Unload = &UnloadMethod<TAsset, TLoader>;
}

template<class TAsset>
const TAsset* eng::AssetManager::ReadAsset(const str::Guid& guid) const
{
	const auto find = m_RefMap.Find(guid);
	if (find != m_RefMap.end())
		return static_cast<const TAsset*>(find->second.m_Asset);
	return nullptr;
}

template<class TAsset>
TAsset* eng::AssetManager::WriteAsset(const str::Guid& guid)
{
	const auto find = m_RefMap.Find(guid);
	if (find != m_RefMap.end())
		return static_cast<TAsset*>(find->second.m_Asset);
	return nullptr;
}

template<class TAsset>
bool eng::AssetManager::ImportFromFile(TAsset& asset, const str::Path& filepath)
{
	constexpr TypeHash typeHash = ToTypeHash<TAsset>();
	const str::Name& type = m_TypeHashs.Get(typeHash);
	const eng::AssetEntry& entry = m_Registry.Get(type);

	asset.m_Guid = str::Guid::Generate();
	asset.m_Name = NAME(filepath.GetFileNameNoExtension());
	asset.m_Type = type;

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	if (!entry.m_Methods.m_Import || !entry.m_Methods.m_Import(asset, *entry.m_Loader, visitor))
	{
		Z_LOG(ELog::Assert, "Asset failed to import from '{}'!", filepath.ToChar());
		return false;
	}

	eng::AssetFile& file = m_FileMap[asset.m_Guid];
	file.m_Guid = asset.m_Guid;
	file.m_Name = asset.m_Name;
	file.m_Path = filepath;
	file.m_Type = type;
	return true;
}

template<class TAsset>
bool eng::AssetManager::LoadFromFile(TAsset& asset, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	eng::Visitor visitor;
	if (!visitor.LoadFromFile(filepath))
		return false;

	visitor.Read("m_Guid", asset.m_Guid, asset.m_Guid);
	visitor.Read("m_Name", asset.m_Name, asset.m_Name);
	visitor.Read("m_Type", asset.m_Type, asset.m_Type);

	const eng::AssetEntry& entry = m_Registry.Get(asset.m_Type);
	if (!entry.m_Methods.m_Load)
		return false;

	return entry.m_Methods.m_Load(asset, *entry.m_Loader, visitor);
}

template<class TAsset>
bool eng::AssetManager::SaveToFile(TAsset& asset, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	constexpr TypeHash typeHash = ToTypeHash<TAsset>();
	const str::Name& type = m_TypeHashs.Get(typeHash);
	const eng::AssetEntry& entry = m_Registry.Get(type);

	if (!asset.m_Guid.IsValid())
		asset.m_Guid = str::Guid::Generate();

	if (asset.m_Name.IsEmpty())
		asset.m_Name = NAME(filepath.GetFileNameNoExtension());

	asset.m_Type = type;

	if (!filepath.HasExtension())
		return false;

	eng::Visitor visitor;
	visitor.Write("m_Guid", asset.m_Guid);
	visitor.Write("m_Name", asset.m_Name);
	visitor.Write("m_Type", asset.m_Type);
	if (!entry.m_Methods.m_Save || !entry.m_Methods.m_Save(asset, *entry.m_Loader, visitor))
	{
		Z_LOG(ELog::Assert, "Asset failed to save to '{}'!", filepath.ToChar());
		return false;
	}

	if (!visitor.SaveToFile(filepath))
		return false;

	eng::AssetFile& file = m_FileMap[asset.m_Guid];
	file.m_Guid = asset.m_Guid;
	file.m_Name = asset.m_Name;
	file.m_Path = filepath;
	file.m_Type = type;
	return true;
}

template<typename TAsset>
void eng::AssetManager::LoadDeferred(const str::Path filepath)
{
	TAsset* asset = new TAsset();
	if (!LoadFromFile(*asset, filepath))
	{
		delete asset;
		return;
	}

	m_Mutex.lock();
	m_Loaded.Append(asset);
	m_Mutex.unlock();
}

template<typename TAsset>
void eng::AssetManager::LoadImmediate(const str::Path& filepath)
{
	TAsset* asset = new TAsset();
	if (!LoadFromFile(*asset, filepath))
	{
		delete asset;
		return;
	}

	BindAsset(asset);
}

template<typename TAsset>
void eng::AssetManager::ScheduleLoad(eng::AssetManager& manager, const str::Path& filepath)
{
	if constexpr (std::is_base_of<eng::DeferredLoad, TAsset>::value)
	{
		std::thread thread(&eng::AssetManager::LoadDeferred<TAsset>, &manager, filepath);
		thread.detach();
	}
	else
	{
		manager.LoadImmediate<TAsset>(filepath);
	}
}

template<typename TAsset, typename TLoader>
void eng::AssetManager::BindMethod(eng::Asset& asset, const eng::AssetLoader& loader)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	tLoader.Bind(tAsset);
}

template<typename TAsset, typename TLoader>
void eng::AssetManager::UnbindMethod(eng::Asset& asset, const eng::AssetLoader& loader)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	tLoader.Unbind(tAsset);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::ImportMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Import(tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::LoadMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Load(tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::SaveMethod(eng::Asset& asset, const eng::AssetLoader& loader, eng::Visitor& visitor)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Save(tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::UnloadMethod(eng::Asset& asset, const eng::AssetLoader& loader)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);
	TAsset& tAsset = static_cast<TAsset&>(asset);
	return tLoader.Unload(tAsset);
}
