#include "EnginePCH.h"
#include "Engine/AssetManager.h"

#include "Core/StringHelpers.h"
#include "Engine/AssetLoader.h"

#include <filesystem>

namespace
{
	const str::StringView strGuid = "m_Guid";
	const str::StringView strName = "m_Name";
	const str::StringView strType = "m_Type";
}

void eng::AssetManager::Initialise()
{
	const str::Path assets = str::GetPath(str::EPath::Assets);
	LoadFilepath(assets, true);
}

void eng::AssetManager::Shutdown()
{
}

bool eng::AssetManager::HasAsset(const str::Guid& guid)
{
	return enumerate::Contains(m_FileMap, guid);
}

void eng::AssetManager::LoadFilepath(const str::Path& filepath, const bool canSearchSubdirectories)
{
	if (filepath.GetFileExtension() == s_Extension)
	{
		// #todo: peak the guid and type
		eng::Visitor visitor;
		if (visitor.LoadFromFile(filepath))
		{
			eng::AssetFile assetFile;
			assetFile.m_Path = filepath;
			visitor.Read(strGuid, assetFile.m_Guid, {});
			visitor.Read(strName, assetFile.m_Name, {});
			visitor.Read(strType, assetFile.m_Type, {});
			m_FileMap[assetFile.m_Guid] = assetFile;
			m_TypeMap[assetFile.m_Type].Add(assetFile.m_Guid);
		}
	}
	else if (filepath.IsDirectory())
	{
		str::Path subpath;
		std::filesystem::path folderPath = filepath.ToChar();
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(folderPath))
		{
			subpath = entry.path().string();
			LoadFilepath(subpath, canSearchSubdirectories);
		}
	}
}

auto eng::AssetManager::GetFileMap() const -> const FileMap&
{
	return m_FileMap;
}

auto eng::AssetManager::GetTypeMap() const -> const TypeMap&
{
	return m_TypeMap;
}

const eng::AssetFile* eng::AssetManager::GetAssetFile(const str::Guid& guid) const
{
	const auto find = m_FileMap.Find(guid);
	return find != m_FileMap.end()
		? &find->second
		: nullptr;
}

void eng::AssetManager::ReloadAssets()
{
	// #temp: easy way of reloading assets is to just clear the cache
	//for (auto&& [typeId, entry] : m_Registry)
	//{
	//	entry.m_Loader->ClearCache();
	//}
}