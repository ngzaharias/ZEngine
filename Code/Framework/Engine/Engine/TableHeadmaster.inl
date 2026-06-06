#pragma once

template<class TManager>
bool eng::TableHeadmaster::IsRegistered() const
{
	const eng::TableId tableId = ToTypeId<TManager, eng::TableTag>();
	return m_Entries.Contains(tableId);
}

template<class TManager, typename... TArgs>
void eng::TableHeadmaster::Register(const str::String& filename, TArgs&&... args)
{
	static_assert(!std::is_const<TManager>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TManager>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TManager>, "Type cannot be a pointer.");

	if (IsRegistered<TManager>())
		return;

	const eng::TableId tableId = ToTypeId<TManager, eng::TableTag>();
	eng::TableEntry& entry = m_Entries.Emplace(tableId);
	entry.m_Name = ToTypeName<TManager>();
	entry.m_Filename = filename;
	entry.m_Manager = new TManager(std::forward<TArgs>(args)...);
}

template<class TManager>
auto eng::TableHeadmaster::GetManager() -> TManager&
{
	static_assert(!std::is_const<TManager>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TManager>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TManager>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TManager>(), "Manager isn't registered!");

	const eng::TableId tableId = ToTypeId<TManager, eng::TableTag>();
	const eng::TableEntry& entry = m_Entries.Get(tableId);
	return *static_cast<TManager*>(entry.m_Manager);
}

template<class TManager>
auto eng::TableHeadmaster::GetManager() const -> const TManager&
{
	static_assert(!std::is_const<TManager>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TManager>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TManager>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TManager>(), "Manager isn't registered!");

	const eng::TableId tableId = ToTypeId<TManager, eng::TableTag>();
	const eng::TableEntry& entry = m_Entries.Get(tableId);
	return *static_cast<TManager*>(entry.m_Manager);
}