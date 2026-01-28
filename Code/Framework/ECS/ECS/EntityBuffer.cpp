#include "ECS/EntityBuffer.h"

//////////////////////////////////////////////////////////////////////////
// Entity

auto ecs::EntityBuffer::CreateEntity() -> ecs::Entity
{
	ecs::Entity entity;
	if (!m_HandlesRecycled.IsEmpty())
	{
		const ecs::Entity recycled = m_HandlesRecycled.GetFirst();
		m_HandlesRecycled.RemoveAt(0);

		entity = ecs::Entity(
			recycled.GetIndex(), 
			recycled.GetVersion() + 1);
	}
	else
	{
		entity = ecs::Entity(m_Index++, 0);
	}

	m_EntityChanges.Insert(entity, ecs::EntityChange());
	return entity;
}

void ecs::EntityBuffer::DestroyEntity(const ecs::Entity& entity)
{
	m_EntityChanges[entity].m_IsDestroy = true;
}

void ecs::EntityBuffer::RecycleEntity(const ecs::Entity& entity)
{
	m_EntityChanges.Remove(entity);
	m_HandlesRecycled.Append(entity);
}

//////////////////////////////////////////////////////////////////////////
// Changes

ecs::EntityBuffer::Changes& ecs::EntityBuffer::GetChanges()
{
	return m_EntityChanges;
}

const ecs::EntityBuffer::Changes& ecs::EntityBuffer::GetChanges() const
{
	return m_EntityChanges;
}

//////////////////////////////////////////////////////////////////////////
// Container

ecs::EntityBuffer::Containers& ecs::EntityBuffer::GetContainers()
{
	return m_Components;
}

const ecs::EntityBuffer::Containers& ecs::EntityBuffer::GetContainers() const
{
	return m_Components;
}

ecs::IComponentContainer& ecs::EntityBuffer::GetContainerAt(const ecs::ComponentId typeId)
{
	return *m_Components.Get(typeId);
}

const ecs::IComponentContainer& ecs::EntityBuffer::GetContainerAt(const ecs::ComponentId typeId) const
{
	return *m_Components.Get(typeId);
}

ecs::IComponentContainer* ecs::EntityBuffer::TryContainerAt(const ecs::ComponentId typeId)
{
	return m_Components.Contains(typeId)
		? m_Components.Get(typeId)
		: nullptr;
}

const ecs::IComponentContainer* ecs::EntityBuffer::TryContainerAt(const ecs::ComponentId typeId) const
{
	return m_Components.Contains(typeId)
		? m_Components.Get(typeId)
		: nullptr;
}