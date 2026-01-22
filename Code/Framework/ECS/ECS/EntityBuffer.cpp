#include "ECS/EntityBuffer.h"

ecs::EntityBuffer::EntityBuffer(const uint8 source)
	: m_HandleNext(1, source, 0)
{
}

auto ecs::EntityBuffer::CreateEntity() -> ecs::Entity
{
	ecs::Entity entity;
	if (!m_HandlesRecycled.IsEmpty())
	{
		const ecs::Entity recycled = m_HandlesRecycled.GetFirst();
		m_HandlesRecycled.RemoveAt(0);

		const uint32 index = recycled.GetIndex();
		const uint32 version = recycled.GetVersion() + 1;
		entity = ecs::Entity(index, version);
	}
	else
	{
		entity = m_HandleNext;
		const uint32 index = entity.GetIndex() + 1;
		const uint32 version = 0;
		m_HandleNext = ecs::Entity(index, version);
	}

	m_EntityChanges.Insert(entity, ecs::EntityChange());
	return entity;
}

void ecs::EntityBuffer::DestroyEntity(const ecs::Entity& entity)
{
	m_EntityChanges[entity].m_IsDestroy = true;
}