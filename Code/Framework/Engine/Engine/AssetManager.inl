#pragma once

#include "Core/Profiler.h"

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
	Z_PANIC(!enumerate::Contains(m_Registry, typeId), "Asset has already been registered! Type [{}] ", ToTypeName<TAsset>());

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
bool eng::AssetManager::SaveAsset(TAsset& asset, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type [{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;

	if (!asset.m_Guid.IsValid())
		asset.m_Guid = str::Guid::Generate();

	str::Path path = filepath;
	if (!path.HasExtension())
		path += s_Extension;

	asset.m_Type = entry.m_Type;

	if (asset.m_Name.IsEmpty())
		asset.m_Name = NAME(filepath.GetFileNameNoExtension());

	if (!entry.m_Save || !entry.m_Save(&asset, loader, path))
	{
		Z_LOG(ELog::Assert, "Asset failed to save! Path [{}]", path.ToChar());
		return false;
	}

	eng::AssetFile& file = m_FileMap[asset.m_Guid];
	file.m_Guid = asset.m_Guid;
	file.m_Name = asset.m_Name;
	file.m_Type = asset.m_Type;
	file.m_Path = path;
	return true;
}

template<class TAsset>
bool eng::AssetManager::LoadAsset(TAsset& asset, const str::Path& filepath)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type [{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;

	if (!entry.m_Load(&asset, loader, filepath))
	{
		Z_LOG(ELog::Assert, "Asset failed to load! Path [{}]", filepath.ToChar());
		return false;
	}

	asset.m_Type = entry.m_Type;
	return true;
}

template<class TAsset>
bool eng::AssetManager::ImportAsset(TAsset& asset, const str::Path& filepath)
{
	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type [{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;

	asset.m_Guid = str::Guid::Generate();
	asset.m_Name = NAME(filepath.GetFileNameNoExtension());
	asset.m_Type = entry.m_Type;

	if (!entry.m_Import || !entry.m_Import(&asset, loader, filepath))
	{
		Z_LOG(ELog::Assert, "Failed to import asset! Path [{}]", filepath.ToChar());
		return false;
	}
	return true;
}

template<class TAsset>
const TAsset* eng::AssetManager::FetchAsset(const str::Guid& guid) const
{
	const auto find = m_RefMap.Find(guid);
	if (find != m_RefMap.end())
		return static_cast<const TAsset*>(find->second.m_Asset);
	return nullptr;
}

template<class TAsset>
TAsset* eng::AssetManager::LoadAsset(const str::Guid& guid)
{
	PROFILE_FUNCTION();

	constexpr TypeId typeId = ToTypeId<TAsset>();
	Z_PANIC(enumerate::Contains(m_Registry, typeId), "Asset hasn't been registered! Type [{}] ", ToTypeName<TAsset>());

	auto& entry = m_Registry[typeId];
	auto& loader = *entry.m_Loader;

	const auto findRef = m_RefMap.Find(guid);
	if (findRef != m_RefMap.end() && findRef->second.m_Asset)
		return static_cast<TAsset*>(findRef->second.m_Asset);

	const auto findFile = m_FileMap.Find(guid);
	if (findFile == m_FileMap.end())
	{
		Z_LOG(ELog::Error, "Asset doesn't exist! Guid [{}]", guid.ToString().c_str());
		return nullptr;
	}

	TAsset* asset = new TAsset();
	if (!entry.m_Load(asset, loader, findFile->second.m_Path))
	{
		Z_LOG(ELog::Error, "Asset failed to load! Path [{}]", findFile->second.m_Path.ToChar());
		delete asset;
		return nullptr;
	}

	asset->m_Guid = guid;
	asset->m_Type = entry.m_Type;
	asset->m_TypeId = typeId;
	return asset;
}

template<class TAsset>
void eng::AssetManager::RequestAsset(const str::Guid& guid)
{
	eng::AssetRef& ref = m_RefMap[guid];

	ref.m_Count++;
	if (ref.m_Count == 1)
	{
		ref.m_Asset = LoadAsset<TAsset>(guid);
	}
}

template<class TAsset>
void eng::AssetManager::ReleaseAsset(const str::Guid& guid)
{
	eng::AssetRef& ref = m_RefMap[guid];

	ref.m_Count--;
	if (ref.m_Count == 0)
	{
		if (ref.m_Asset)
		{
			delete ref.m_Asset;
			ref.m_Asset = nullptr;
		}
		m_RefMap.Remove(guid);
	}
	else
	{
		Z_PANIC(ref.m_Count >= 0, "Reference count dropped below 0! Guid [{}]", guid.ToString());
	}
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::SaveFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	Z_PANIC(!filepath.IsEmpty(), "Invalid filepath! Path [{}]", filepath.ToChar());

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

template<typename TAsset, typename TLoader>
bool eng::AssetManager::LoadFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	Z_PANIC(!filepath.IsEmpty(), "Invalid filepath! Path [{}]", filepath.ToChar());

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read("m_Guid", asset->m_Guid, asset->m_Guid);
	visitor.Read("m_Name", asset->m_Name, asset->m_Name);
	visitor.Read("m_Type", asset->m_Type, asset->m_Type);

	const TLoader* tLoader = static_cast<const TLoader*>(&loader);
	TAsset* tAsset = static_cast<TAsset*>(asset);

	return tLoader->Load(*tAsset, visitor);
}

template<typename TAsset, typename TLoader>
bool eng::AssetManager::ImportFunction(eng::Asset* asset, const eng::AssetLoader& loader, const str::Path& filepath)
{
	Z_PANIC(!filepath.IsEmpty(), "Invalid filepath! Path [{}]", filepath.ToChar());

	const TLoader* tLoader = static_cast<const TLoader*>(&loader);
	TAsset* tAsset = static_cast<TAsset*>(asset);
	return tLoader->Import(*tAsset, filepath);
}