#include "EnginePCH.h"
#include "Engine/TableHeadmaster.h"

#include "Core/Guid.h"
#include "Core/Path.h"
#include "Engine/TableManager.h"
#include "Engine/Visitor.h"

void eng::TableHeadmaster::Initialise(const str::Path& folderPath)
{
	PROFILE_FUNCTION();

	m_FolderPath = folderPath;
	for (const eng::TableEntry& entry : m_Entries.GetValues())
	{
		str::Path filepath = str::Path(m_FolderPath, "\\", entry.m_Filename);
		if (!filepath.HasExtension())
			filepath += s_Extension;

		eng::Visitor visitor;
		if (visitor.LoadFromFile(filepath))
			entry.m_Manager->Read(visitor);
	}
}

void eng::TableHeadmaster::Shutdown()
{
	PROFILE_FUNCTION();

	for (const eng::TableEntry& entry : m_Entries.GetValues())
		delete entry.m_Manager;
	m_Entries.RemoveAll();
}

void eng::TableHeadmaster::Reload()
{
	Initialise(m_FolderPath);
}