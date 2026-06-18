#pragma once

template<class TImporter>
bool editor::importer::Manager::IsRegistered() const
{
	const TypeId typeId = ToTypeId<TImporter, Tag>();
	return m_Entries.Contains(typeId);
}

template<class TImporter, typename... TArgs>
void editor::importer::Manager::Register(TArgs&&... args)
{
	const TypeId typeId = ToTypeId<TImporter, Tag>();

	Entry& entry = m_Entries.Emplace(typeId);
	entry.m_Importer = new TImporter(std::forward<TArgs>(args)...);
	entry.m_TypeId = typeId;
}

template<class TImporter>
TImporter& editor::importer::Manager::Get()
{
	const TypeId typeId = ToTypeId<TImporter, Tag>();
	const Entry& entry = m_Entries.Get(typeId);
	return *static_cast<TImporter*>(entry.m_Importer);
}