#pragma once

#include "Core/Array.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "Core/StringView.h"

namespace eng
{
	struct SaveFileSettings
	{
		str::String m_Title = "Save File";
		str::String m_Path = "";
		Array<str::String> m_Filters = { "All Files (*.*)", "*" };

		bool m_Overwrite = false;
	};

	struct SelectFileSettings
	{
		str::String m_Title = "Select File";
		str::String m_Path = "";
		Array<str::String> m_Filters = { "All Files (*.*)", "*" };
	};

	struct SelectFilesSettings
	{
		str::String m_Title = "Select File(s)";
		str::String m_Path = "";
		Array<str::String> m_Filters = { "All Files (*.*)", "*" };
	};

	struct SelectFolderSettings
	{
		str::String m_Title = "Select Folder";
		str::String m_Path = "";
	};

	struct SearchDirectorySettings
	{
		str::StringView m_Extension = {};
		bool m_IsRecursive = true;
	};

	str::Path GetAppDataDirectory();
	str::Path GetAssetsDirectory();
	str::Path GetConfigDirectory();
	str::Path GetCurrentFilepath();
	str::Path GetExecutableFilepath();
	str::Path GetLevelsDirectory();
	str::Path GetThirdPartyDirectory();
	str::Path GetWorkingDirectory();

	bool LoadFileAsBinary(const str::Path& filepath, str::String& out_String);
	bool LoadFileAsChar(const str::Path& filepath, Array<char>& out_Array);

	str::Path SaveFileDialog(const SaveFileSettings& settings);
	str::Path SelectFileDialog(const SelectFileSettings& settings);
	Array<str::Path> SelectFilesDialog(const SelectFilesSettings& settings);
	str::Path SelectFolderDialog(const SelectFolderSettings& settings);

	Array<str::Path> SearchDirectory(const str::Path& directory, const SearchDirectorySettings& settings);
}