#pragma once

namespace _private
{
	template<typename TAsset, typename TLoader>
	using HasImportMethod = decltype(std::declval<TLoader>().Import(std::declval<TAsset*>(), std::declval<const str::Path&>()));
	template<typename TAsset, typename TLoader>
	using HasSaveMethod = decltype(std::declval<TLoader>().Save(std::declval<TAsset*>(), std::declval<eng::Visitor&>()));
}

template<typename TAsset, typename TLoader, typename... TArgs>
void eng::AssetManager::RegisterAsset(const str::Name& type, TArgs&&... args)
{
	static_assert(std::is_convertible<TAsset*, eng::Asset*>::value, "Asset must inherit from eng::Asset using the [public] keyword!");
	static_assert(std::is_base_of<eng::Asset, TAsset>::value, "Asset isn't a base of eng::Asset!");

	static_assert(std::is_convertible<TLoader*, eng::AssetLoader*>::value, "Loader must inherit from eng::AssetLoader using the [public] keyword!");
	static_assert(std::is_base_of<eng::AssetLoader, TLoader>::value, "Loader isn't a base of eng::AssetLoader!");

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(!core::Contains(m_EntryMap, typeId), "Asset type has already been registered! TypeId[{}]", typeId);

	eng::AssetEntry& entry = m_EntryMap[typeId];
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
const TAsset* eng::AssetManager::ImportAsset(const str::Path& inputPath, const str::Path& outputPath)
{
	constexpr TypeId typeId = ToTypeId<TAsset>();
	const str::Guid guid = str::Guid::Generate();
	Z_PANIC(core::Contains(m_EntryMap, typeId), "Asset type hasn't been registered! TypeId[{}]", typeId);
	Z_PANIC(!core::Contains(m_FileMap, guid), "Duplicate Asset already exists! Guid[{}]", guid.ToString().c_str());

	auto& entry = m_EntryMap[typeId];
	auto& loader = *entry.m_Loader;
	auto& import = entry.m_Import;
	auto& save = entry.m_Save;

	TAsset* asset = new TAsset();
	asset->m_Guid = guid;
	asset->m_Name = NAME(outputPath.GetFileNameNoExtension());
	asset->m_Path = outputPath;
	asset->m_Type = entry.m_Type;

	if (!import(asset, loader, inputPath))
	{
		Z_LOG(ELog::Assert, "Cannot import asset! Guid[{}] Path[{}]", guid.ToString().c_str(), inputPath.ToChar());
		delete asset;
		return nullptr;
	}

	if (!save(asset, loader, outputPath))
	{
		Z_LOG(ELog::Assert, "Cannot save asset! Guid[{}] Path[{}]", guid.ToString().c_str(), outputPath.ToChar());
		delete asset;
		return nullptr;
	}

	if (!load(asset, loader))
	{
		Z_LOG(ELog::Assert, "Cannot load asset after importing! Guid[{}] Path[{}]", guid.ToString().c_str(), outputPath.ToChar());
		delete asset;
		return nullptr;
	}

	m_FileMap.Emplace(guid, eng::AssetFile(guid, outputPath, entry.m_Type));

	auto& cache = loader.m_Cache;
	cache[guid] = asset;
	return asset;
}

template<class TAsset>
const TAsset* eng::AssetManager::LoadAsset(const str::Guid& guid)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(core::Contains(m_EntryMap, typeId), "Asset type hasn't been registered! TypeId[{}]", typeId);

	auto& entry = m_EntryMap[typeId];
	auto& loader = *entry.m_Loader;
	auto& cache = loader.m_Cache;

	const auto find = cache.Find(guid);
	if (find != cache.end())
		return dynamic_cast<const TAsset*>(find->second);

	Z_PANIC(core::Contains(m_FileMap, guid), "Asset doesn't exist! Guid[{}]", guid.ToString().c_str());
	const eng::AssetFile& file = m_FileMap.Get(guid);

	TAsset* asset = new TAsset();
	asset->m_Guid = guid;
	asset->m_Path = file.m_Path;
	asset->m_Type = file.m_Type;

	auto& load = entry.m_Load;
	if (!load(asset, loader))
	{
		Z_LOG(ELog::Assert, "Cannot load Asset! Guid[{}] Path[{}]", asset->m_Guid.ToString().c_str(), asset->m_Path.ToChar());
		delete asset;
		return nullptr;
	}

	cache[guid] = asset;
	return asset;
}

template<class TAsset>
bool eng::AssetManager::SaveAsset(TAsset& asset, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(core::Contains(m_EntryMap, typeId), "Asset type hasn't been registered! TypeId[{}]", typeId);

	auto& entry = m_EntryMap[typeId];
	auto& save = entry.m_Save;
	auto& loader = *entry.m_Loader;

	asset.m_Guid = str::Guid::Generate();
	asset.m_Path = filepath;
	if (asset.m_Path.GetFileExtension() != s_Extension)
		asset.m_Path += s_Extension;
	asset.m_Type = entry.m_Type;
	if (asset.m_Name.IsEmpty())
		asset.m_Name = NAME(filepath.GetFileNameNoExtension());

	if (!save(&asset, loader))
	{
		Z_LOG(ELog::Assert, "Cannot save Asset! Guid[{}] Path[{}]", asset.m_Guid.ToString().c_str(), asset.m_Path.ToChar());
		return false;
	}

	m_FileMap.Insert(asset.m_Guid, eng::AssetFile(asset.m_Guid, asset.m_Path, asset.m_Type));
	return true;
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::ImportFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	const TLoader* tLoader = dynamic_cast<const TLoader*>(&loader);
	TAsset* tAsset = dynamic_cast<TAsset*>(asset);
	return tLoader->Import(tAsset, filepath);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::LoadFunction(eng::Asset* asset, const eng::AssetLoader& loader)
{
	Z_PANIC(!asset->m_Path.IsEmpty(), "Invalid Path [{}]", asset->m_Path.ToChar());

	eng::Visitor visitor;
	visitor.LoadFromFile(asset->m_Path);
	visitor.Visit("m_Name", asset->m_Name, {});

	const TLoader* tLoader = dynamic_cast<const TLoader*>(&loader);
	TAsset* tAsset = dynamic_cast<TAsset*>(asset);

	return tLoader->Load(tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::SaveFunction(eng::Asset* asset, const eng::AssetLoader& loader)
{
	Z_PANIC(!asset->m_Path.IsEmpty(), "Invalid Path [{}]", asset->m_Path.ToChar());

	eng::Visitor visitor;
	visitor.SetMode(eng::Visitor::Write);
	visitor.Visit("m_Guid", asset->m_Guid, {});
	visitor.Visit("m_Name", asset->m_Name, {});
	visitor.Visit("m_Type", asset->m_Type, {});

	const TLoader* tLoader = dynamic_cast<const TLoader*>(&loader);
	TAsset* tAsset = dynamic_cast<TAsset*>(asset);

	if (!tLoader->Save(tAsset, visitor))
		return false;

	return visitor.SaveToFile(asset->m_Path);
}