#include "EnginePCH.h"
#include "Engine/TableHeadmaster.h"

#include "Core/Guid.h"
#include "Core/Path.h"
#include "Engine/TableManager.h"
#include "Engine/Visitor.h"

void eng::TableHeadmaster::Initialise(const str::Path& folderPath)
{
	for (const eng::TableEntry& entry : m_Entries.GetValues())
	{
		str::Path filepath = str::Path(folderPath, "\\", entry.m_Filename);
		if (!filepath.HasExtension())
			filepath += s_Extension;

		eng::Visitor visitor;
		if (visitor.LoadFromFile(filepath))
			entry.m_Manager->Visit(visitor);
	}
}

void eng::TableHeadmaster::Shutdown()
{
	for (const eng::TableEntry& entry : m_Entries.GetValues())
		delete entry.m_Manager;
	m_Entries.RemoveAll();
}