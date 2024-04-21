#pragma once

#include <Core/Name.h>
#include <Core/TypeInfo.h>

template<class TManager>
bool eng::TableHeadmaster::IsRegistered() const
{
	using NonConst = std::remove_const<TManager>::type;
	const eng::TableId tableId = ToTypeIndex<NonConst, eng::TableTag>();
	return m_Entries.Contains(tableId);
}

template<class TManager, typename... TArgs>
void eng::TableHeadmaster::Register(const str::String& filename, TArgs&&... args)
{
	Z_PANIC(!IsRegistered<TManager>(), "TableManager has already been registered!");

	using NonConst = std::remove_const<TManager>::type;
	const eng::TableId tableId = ToTypeIndex<NonConst, eng::TableTag>();

	eng::TableEntry& entry = m_Entries.Emplace(tableId);
	entry.m_Name = ToTypeName<NonConst>();
	entry.m_Filename = filename;
	entry.m_Manager = new TManager(std::forward<TArgs>(args)...);
}

template<class TManager>
auto eng::TableHeadmaster::GetManager() -> TManager&
{
	Z_PANIC(IsRegistered<TManager>(), "TableManager hasn't been registered!");

	using NonConst = std::remove_const<TManager>::type;
	const eng::TableId tableId = ToTypeIndex<NonConst, eng::TableTag>();
	const eng::TableEntry& entry = m_Entries.Get(tableId);
	return *static_cast<TManager*>(entry.m_Manager);
}

template<class TManager>
auto eng::TableHeadmaster::GetManager() const -> const TManager&
{
	Z_PANIC(IsRegistered<TManager>(), "TableManager hasn't been registered!");

	using NonConst = std::remove_const<TManager>::type;
	const eng::TableId tableId = ToTypeIndex<NonConst, eng::TableTag>();
	const eng::TableEntry& entry = m_Entries.Get(tableId);
	return *static_cast<TManager*>(entry.m_Manager);
}