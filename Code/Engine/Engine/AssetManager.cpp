#include "EnginePCH.h"
#include "Engine/AssetManager.h"

#include "Core/StringHelpers.h"

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
			m_FileMap.Insert(assetFile.m_Guid, assetFile);
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
