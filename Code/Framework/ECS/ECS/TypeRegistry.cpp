#include "ECS/TypeRegistry.h"

const ecs::TypeRegistry::TypeMap& ecs::TypeRegistry::GetTypeMap() const
{
	return m_TypeMap;
}

const ecs::TypeInfo& ecs::TypeRegistry::GetTypeInfo(const TypeId typeId) const
{
	return m_TypeMap.Get(typeId);
}

const ecs::TypeRegistry::ComponentMap& ecs::TypeRegistry::GetComponentMap() const
{
	return m_ComponentMap;
}

const ecs::TypeComponent& ecs::TypeRegistry::GetComponentInfo(const ecs::ComponentId& typeId) const
{
	return m_ComponentMap.Get(typeId);
}

const ecs::TypeComponent* ecs::TypeRegistry::TryComponentInfo(const ecs::ComponentId& typeId) const
{
	const auto find = m_ComponentMap.Find(typeId);
	return find != m_ComponentMap.end()
		? &find->second
		: nullptr;
}

const ecs::TypeEvent& ecs::TypeRegistry::GetTypeEvent(const ecs::EventId& typeId) const
{
	return m_EventMap.Get(typeId);
}

const ecs::TypeEvent* ecs::TypeRegistry::TryTypeEvent(const ecs::EventId& typeId) const
{
	const auto find = m_EventMap.Find(typeId);
	return find != m_EventMap.end() 
		? &find->second 
		: nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Component

bool ecs::TypeRegistry::HasComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	return entry.m_HasSolo(storage, entity);
}

void ecs::TypeRegistry::AddComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_AddSolo(storage, entity);
}

void ecs::TypeRegistry::AddComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_AddData(storage, entity, data);
}

void ecs::TypeRegistry::UpdateComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_UpdateSolo(storage, entity);
}

void ecs::TypeRegistry::UpdateComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_UpdateData(storage, entity, data);
}

void ecs::TypeRegistry::RemoveComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_RemoveSolo(storage, entity);
}

void ecs::TypeRegistry::ReadComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, MemBuffer& data) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_ReadData(storage, entity, data);
}

void ecs::TypeRegistry::WriteComponent(ecs::EntityStorage& storage, const ecs::ComponentId typeId, const ecs::Entity& entity, const MemBuffer& data) const
{
	const ecs::TypeComponent& entry = m_ComponentMap.Get(typeId);
	entry.m_WriteData(storage, entity, data);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::TypeRegistry::AddEvent(ecs::EventBuffer& buffer, const ecs::EventId typeId, const MemBuffer& data) const
{
	const ecs::TypeEvent& entry = m_EventMap.Get(typeId);
	entry.m_Add(buffer, data);
}