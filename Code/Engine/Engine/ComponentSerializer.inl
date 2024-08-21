#pragma once

#include "Engine/ReplicationComponents.h"

namespace net
{
	template<typename TComponent, typename TSerializer>
	void AddComponent(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer)
	{
		auto& component = world.AddComponent<TComponent>(entity);
		TSerializer::Read(component, buffer);
	}

	template<typename TComponent, typename TSerializer>
	void RemoveComponent(ecs::EntityWorld& world, const ecs::Entity& entity)
	{
		world.RemoveComponent<TComponent>(entity);
	}

	template<typename TComponent, typename TSerializer>
	void ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer)
	{
		auto& component = world.WriteComponent<TComponent>(entity);
		TSerializer::Read(component, buffer);
	}

	template<typename TComponent, typename TSerializer>
	void WriteComponent(ecs::EntityWorld& world, const ecs::Entity& entity, MemBuffer& buffer)
	{
		const auto& component = world.ReadComponent< TComponent>(entity);
		TSerializer::Write(component, buffer);
	}
}

template<typename TComponent, typename TSerializer>
void net::ComponentSerializer::RegisterComponent()
{
	const ecs::ComponentId worldId = ToTypeIndex<TComponent, ecs::ComponentTag>();
	const ecs::ComponentId localId = ToTypeIndex<TComponent, net::ComponentTag>();
	const TypeId typeId = ToTypeId<TComponent>();

	if (localId >= m_Entries.GetCount())
		m_Entries.Resize(localId + 1);

	net::ComponentEntry& entry = m_Entries[localId];
	entry.m_Add    = &AddComponent<TComponent, TSerializer>;
	entry.m_Remove = &RemoveComponent<TComponent, TSerializer>;
	entry.m_Read   = &ReadComponent<TComponent, TSerializer>;
	entry.m_Write  = &WriteComponent<TComponent, TSerializer>;
	entry.m_AddedId = ecs::QueryProxy<ecs::query::Added<TComponent>::Include<net::ReplicationComponent>>::Id();
	entry.m_RemovedId = ecs::QueryProxy<ecs::query::Removed<TComponent>::Include<net::ReplicationComponent>>::Id();
	entry.m_UpdatedId = ecs::QueryProxy<ecs::query::Updated<TComponent>::Include<net::ReplicationComponent>>::Id();
	entry.m_ComponentId = localId;

	m_TypeIdToLocal[typeId] = localId;
	m_WorldToLocal[worldId] = localId;
}