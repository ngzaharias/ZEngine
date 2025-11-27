#pragma once

template<class TSingleton>
void ecs::SingletonRegistry::Register()
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(std::is_base_of<ecs::Singleton<TSingleton>, TSingleton>::value, "Type doesn't inherit from ecs::Singleton.");
	static_assert(std::is_convertible<TSingleton*, ecs::Singleton<TSingleton>*>::value, "Singleton must inherit from ecs::Singleton using the [public] keyword!");

	Z_PANIC(!IsRegistered<TSingleton>(), "Singleton is already registered!");

	const ecs::SingletonId id = ToTypeIndex<TSingleton, ecs::SingletonTag>();
	ecs::SingletonEntry& entry = m_Entries.Emplace(id);
	entry.m_Name = ToTypeName<TSingleton>();
}

template<class TSingleton>
bool ecs::SingletonRegistry::IsRegistered() const
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");

	const ecs::SingletonId id = ToTypeIndex<TSingleton, ecs::SingletonTag>();
	return m_Entries.Contains(id);
}

template<class TSingleton>
auto ecs::SingletonRegistry::GetEntry() const -> const ecs::SingletonEntry&
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	Z_PANIC(IsRegistered<TSingleton>(), "Singleton isn't registered!");

	const ecs::SingletonId id = ToTypeIndex<TSingleton, ecs::SingletonTag>();
	return m_Entries.Get(id);
}