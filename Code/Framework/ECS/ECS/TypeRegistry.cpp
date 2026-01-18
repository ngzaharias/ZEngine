#include "ECS/TypeRegistry.h"

const ecs::TypeEvent& ecs::TypeRegistry::GetTypeEvent(const ecs::EventId& typeId) const
{
	return m_EventMap.Get(typeId);
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::TypeRegistry::AddComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& buffer) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_Add(world, entity, buffer);
}

void ecs::TypeRegistry::UpdateComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& buffer) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_Write(world, entity, buffer);
}

void ecs::TypeRegistry::RemoveComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_Remove(world, entity);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::TypeRegistry::AddEvent(ecs::EntityWorld& world, const ecs::EventId typeId, const MemBuffer& buffer) const
{
	const ecs::TypeEvent& entry = m_EventMap.Get(typeId);
	entry.m_Add(world, buffer);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

void ecs::TypeRegistry::UpdateSingleton(ecs::EntityWorld& world, const ecs::SingletonId typeId, const MemBuffer& buffer) const
{
	const ecs::TypeSingleton& entry = m_SingletonMap.Get(typeId);
	entry.m_Write(world, buffer);
}