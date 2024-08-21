#pragma once

namespace _private
{
	template<typename TAsset, typename TLoader>
	using HasImportMethod = decltype(std::declval<TLoader>().Import(std::declval<TAsset&>(), std::declval<const str::Path&>()));
	template<typename TAsset, typename TLoader>
	using HasSaveMethod = decltype(std::declval<TLoader>().Save(std::declval<TAsset&>(), std::declval<eng::Visitor&>()));
}

template<typename TAsset, typename TLoader, typename... TArgs>
void eng::AssetManager::RegisterAsset(const str::Name& type, TArgs&&... args)
{
	static_assert(std::is_convertible<TAsset*, eng::Asset*>::value, "Asset must inherit from eng::Asset using the [public] keyword!");
	static_assert(std::is_base_of<eng::Asset, TAsset>::value, "Asset isn't a base of eng::Asset!");

	static_assert(std::is_convertible<TLoader*, eng::AssetLoader*>::value, "Loader must inherit from eng::AssetLoader using the [public] keyword!");
	static_assert(std::is_base_of<eng::AssetLoader, TLoader>::value, "Loader isn't a base of eng::AssetLoader!");

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(!enumerate::Contains(m_Registry, typeId), "Asset has already been registered! Type[{}] ", ToTypeName<TAsset>());

	eng::AssetEntry& entry = m_Registry[typeId];
	entry.m_Type = type;
	entry.m_Loader = new TLoader(std::forward<TArgs>(args)...);
	entry.m_Loader->m_AssetManager = this;

	if constexpr (core::IsDetected<_private::HasImportMethod, TAsset, TLoader>::value)
		entry.m_Import = &ImportFunction<TAsset, TLoader>;

	// all assets must be loadable
	entry.m_Load = &LoadFunction<TAsset, TLoader>;

	// #todo: required
	if constexpr (core::IsDetected<_private::HasSaveMethod, TAsset, TLoader>::value)
		entry.m_Save = &SaveFunction<TAsset, TLoader>;

}

template<class TAsset>
bool eng::AssetManager::ImportAsset(TAsset& asset, const str::Path& filepath)
{
	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type[{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;

	asset.m_Guid = str::Guid::Generate();
	asset.m_Name = NAME(filepath.GetFileNameNoExtension());
	asset.m_Type = entry.m_Type;

	if (!entry.m_Import || !entry.m_Import(&asset, loader, filepath))
	{
		Z_LOG(ELog::Assert, "Failed to import asset! Path[{}]", filepath.ToChar());
		return false;
	}

	return true;
}

template<class TAsset>
const TAsset* eng::AssetManager::LoadAsset(const str::Guid& guid)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type[{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;
	auto& cache = loader.m_Cache;

	const auto find = cache.Find(guid);
	if (find != cache.end())
		return static_cast<const TAsset*>(find->second);

	Z_PANIC(enumerate::Contains(m_FileMap, guid), "Asset doesn't exist! Guid[{}]", guid.ToString().c_str());
	const eng::AssetFile& file = m_FileMap.Get(guid);

	TAsset* asset = new TAsset();
	asset->m_Guid = guid;
	asset->m_Type = file.m_Type;

	if (!entry.m_Load(asset, loader, file.m_Path))
	{
		Z_LOG(ELog::Assert, "Asset failed to load! Path[{}]", file.m_Path.ToChar());
		delete asset;
		return nullptr;
	}

	cache[guid] = asset;
	return asset;
}

template<class TAsset>
bool eng::AssetManager::SaveAsset(TAsset& asset, str::Path filepath)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type[{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;

	if (!asset.m_Guid.IsValid())
		asset.m_Guid = str::Guid::Generate();

	if (!filepath.HasExtension())
		filepath += s_Extension;
	if (filepath.GetFileExtension() != s_Extension)
	{
		Z_LOG(ELog::Assert, "Invalid asset extension! Path[{}]", filepath.ToChar());
		return false;
	}

	asset.m_Type = entry.m_Type;

	if (asset.m_Name.IsEmpty())
		asset.m_Name = NAME(filepath.GetFileNameNoExtension());

	if (!entry.m_Save || !entry.m_Save(&asset, loader, filepath))
	{
		Z_LOG(ELog::Assert, "Asset failed to save! Path[{}]", filepath.ToChar());
		return false;
	}

	eng::AssetFile& file = m_FileMap[asset.m_Guid];
	file.m_Guid = asset.m_Guid;
	file.m_Name = asset.m_Name;
	file.m_Type = asset.m_Type;
	return true;
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::ImportFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	Z_PANIC(!filepath.IsEmpty(), "Invalid filepath! Path[{}]", filepath.ToChar());

	const TLoader* tLoader = static_cast<const TLoader*>(&loader);
	TAsset* tAsset = static_cast<TAsset*>(asset);
	return tLoader->Import(*tAsset, filepath);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::LoadFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	Z_PANIC(!filepath.IsEmpty(), "Invalid filepath! Path[{}]", filepath.ToChar());

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read("m_Name", asset->m_Name, {});

	const TLoader* tLoader = static_cast<const TLoader*>(&loader);
	TAsset* tAsset = static_cast<TAsset*>(asset);

	return tLoader->Load(*tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::SaveFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	Z_PANIC(!filepath.IsEmpty(), "Invalid filepath! Path[{}]", filepath.ToChar());

	eng::Visitor visitor;
	visitor.Write("m_Guid", asset->m_Guid);
	visitor.Write("m_Name", asset->m_Name);
	visitor.Write("m_Type", asset->m_Type);

	const TLoader* tLoader = static_cast<const TLoader*>(&loader);
	TAsset* tAsset = static_cast<TAsset*>(asset);

	if (!tLoader->Save(*tAsset, visitor))
		return false;

	return visitor.SaveToFile(filepath);
}