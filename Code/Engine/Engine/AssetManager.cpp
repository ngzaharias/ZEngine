#include "EnginePCH.h"
#include "Engine/AssetManager.h"

#include <Core/StringHelpers.h>

#include <filesystem>

namespace
{
	constexpr const char* s_Extension = ".asset";

	const str::Name strGuid = NAME("m_Guid");
	const str::Name strType = NAME("m_Type");
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
			str::Guid guid;
			str::Name type;
			visitor.Visit(strGuid, guid, {});
			visitor.Visit(strType, type, {});
			m_FileMap.Insert(guid, eng::AssetFile(guid, filepath, type));
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
