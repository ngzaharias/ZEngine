#pragma once

template<class TSingleton>
void ecs::SingletonRegistry::Register()
{
	const ecs::SingletonId id = ToTypeId<TSingleton, ecs::SingletonTag>();
	ecs::SingletonEntry& entry = m_Entries.Emplace(id);
	entry.m_Name = ToTypeName<TSingleton>();
}

template<class TSingleton>
bool ecs::SingletonRegistry::IsRegistered() const
{
	const ecs::SingletonId id = ToTypeId<TSingleton, ecs::SingletonTag>();
	return m_Entries.Contains(id);
}

template<class TSingleton>
auto ecs::SingletonRegistry::GetEntry() const -> const ecs::SingletonEntry&
{
	const ecs::SingletonId id = ToTypeId<TSingleton, ecs::SingletonTag>();
	return m_Entries.Get(id);
}