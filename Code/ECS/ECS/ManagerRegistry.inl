#pragma once

template<class TManager>
void ecs::ManagerRegistry::Register(TManager& manager)
{
	static_assert(std::is_convertible<TManager*, ecs::Manager*>::value, "Manager must inherit from ecs::Manager using the [public] keyword!");
	static_assert(std::is_base_of<ecs::Manager, TManager>::value, "Type doesn't inherit from ecs::Manager.");
	Z_PANIC(!IsRegistered<TManager>(), "Manager has already been registered!");

	ecs::ManagerEntry entry;
	entry.m_Manager = &manager;
	entry.m_ManagerId = ToTypeIndex<TManager, ecs::ManagerTag>();
	entry.m_DebugName = ToTypeName<TManager>();
	entry.m_IsExternal = true;

	m_Entries.Set(entry.m_ManagerId, std::move(entry));
}

template<class TManager, typename... TArgs>
void ecs::ManagerRegistry::Register(TArgs&&... args)
{
	static_assert(std::is_convertible<TManager*, ecs::Manager*>::value, "Manager must inherit from ecs::Manager using the [public] keyword!");
	static_assert(std::is_base_of<ecs::Manager, TManager>::value, "Type doesn't inherit from ecs::Manager.");
	Z_PANIC(!IsRegistered<TManager>(), "Manager has already been registered!");

	ecs::ManagerEntry entry;
	entry.m_Manager = new TManager(std::forward<TArgs>(args)...);
	entry.m_ManagerId = ToTypeIndex<TManager, ecs::ManagerTag>();
	entry.m_DebugName = ToTypeName<TManager>();
	entry.m_IsExternal = false;

	m_Entries.Set(entry.m_ManagerId, std::move(entry));
}

template<class TManager>
bool ecs::ManagerRegistry::IsRegistered() const
{
	using NonConst = std::remove_const<TManager>::type;
	const ecs::ManagerId managerId = ToTypeIndex<NonConst, ecs::ManagerTag>();
	return m_Entries.Contains(managerId);
}

template<class TManager>
TManager& ecs::ManagerRegistry::GetManager()
{
	Z_PANIC(IsRegistered<TManager>(), "Manager hasn't been registered!");

	using NonConst = std::remove_const<TManager>::type;
	const ecs::ManagerId managerId = ToTypeIndex<NonConst, ecs::ManagerTag>();
	const ecs::ManagerEntry& entry = m_Entries.Get(managerId);
	return *static_cast<NonConst*>(entry.m_Manager);
}