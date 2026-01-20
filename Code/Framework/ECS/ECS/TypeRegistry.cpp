#include "ECS/TypeRegistry.h"

const ecs::TypeEvent& ecs::TypeRegistry::GetTypeEvent(const ecs::EventId& typeId) const
{
	return m_EventMap.Get(typeId);
}

const ecs::TypeComponent& ecs::TypeRegistry::GetTypeComponent(const ecs::ComponentId& typeId) const
{
	return m_ComponentMap.Get(typeId);
}

const ecs::TypeEvent* ecs::TypeRegistry::TryTypeEvent(const ecs::EventId& typeId) const
{
	const auto find = m_EventMap.Find(typeId);
	return find != m_EventMap.end() 
		? &find->second 
		: nullptr;
}

const ecs::TypeComponent* ecs::TypeRegistry::TryTypeComponent(const ecs::ComponentId& typeId) const
{
	const auto find = m_ComponentMap.Find(typeId);
	return find != m_ComponentMap.end()
		? &find->second
		: nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::TypeRegistry::AddComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_Add(world, entity, data);
}

void ecs::TypeRegistry::UpdateComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_Write(world, entity, data);
}

void ecs::TypeRegistry::RemoveComponent(ecs::EntityWorld& world, const ecs::ComponentId typeId, const ecs::Entity& entity) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_Remove(world, entity);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::TypeRegistry::AddEvent(ecs::EventBuffer& buffer, const ecs::EventId typeId, const MemBuffer& data) const
{
	const ecs::TypeEvent& entry = m_EventMap.Get(typeId);
	entry.m_Add(buffer, data);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

void ecs::TypeRegistry::UpdateSingleton(ecs::EntityWorld& world, const ecs::SingletonId typeId, const MemBuffer& data) const
{
	const ecs::TypeSingleton& entry = m_SingletonMap.Get(typeId);
	entry.m_Write(world, data);
}