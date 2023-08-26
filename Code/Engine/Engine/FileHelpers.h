#pragma once

#include <Core/Array.h>
#include <Core/Path.h>
#include <Core/String.h>

namespace eng
{
	struct SaveFileSettings
	{
		str::String m_Title = "Save File";
		str::String m_Directory = "";
		Array<str::String> m_Filters = { "All Files (*.*)", "*" };
	};

	struct SelectFileSettings
	{
		str::String m_Title = "Select File(s)";
		str::String m_Directory = "";
		Array<str::String> m_Filters = { "All Files (*.*)", "*" };
		bool m_IsMultiSelect = false;
	};

	struct SelectFolderSettings
	{
		str::String m_Title = "Select Folder";
		str::String m_Directory = "";
	};

	str::Path GetAppDataDirectory();
	str::Path GetAssetDirectory();
	str::Path GetExecutableFilepath();
	str::Path GetWorkingDirectory();

	bool LoadFileAsBinary(const str::Path& filepath, str::String& out_String);
	bool LoadFileAsChar(const str::Path& filepath, Array<char>& out_Array);

	str::Path SaveFileDialog(const SaveFileSettings& settings);
	Array<str::Path> SelectFileDialog(const SelectFileSettings& settings);
	str::Path SelectFolderDialog(const SelectFolderSettings& settings);
}