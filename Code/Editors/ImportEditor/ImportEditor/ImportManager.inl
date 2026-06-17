#pragma once

template<class TImporter>
bool editor::importer::ImportManager::IsRegistered() const
{
	const ecs::ImporterId resourceId = ToTypeId<TImporter, ecs::ImporterTag>();
	return m_Entries.Contains(resourceId);
}

template<class TImporter, typename... TArgs>
void editor::importer::ImportManager::Register(TArgs&&... args)
{
	const TypeId importerId = ToTypeId<TImporter, ecs::ImporterTag>();

	ImportEntry& entry = m_Entries.Emplace(importerId);
	entry.m_System = new TImporter(std::forward<TArgs>(args)...);
	entry.m_TypeId = importerId;
}

template<class TImporter>
TImporter& editor::importer::ImportManager::Get()
{
	const ecs::ImporterId resourceId = ToTypeId<TImporter, ecs::ImporterTag>();
	const ecs::ImporterEntry& entry = m_Entries.Get(resourceId);
	return *static_cast<TImporter*>(entry.m_Importer);
}