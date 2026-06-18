#include "ImportPCH.h"
#include "ImportEditor/ImportManager.h"

#include "ImportEditor/ImportEntry.h"
#include "ImportEditor/ImportImporter.h"

editor::importer::Manager::Manager()
{
}

editor::importer::Manager::~Manager()
{
	for (auto&& [typeId, entry] : m_Entries)
		delete entry.m_Importer;
	m_Entries.RemoveAll();
}

