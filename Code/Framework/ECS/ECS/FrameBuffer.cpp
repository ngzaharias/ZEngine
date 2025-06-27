#include "ECS/FrameBuffer.h"

auto ecs::FrameBuffer::CreateEntity()->ecs::Entity
{
	ecs::Entity entity = { };
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
		entity = m_HandlesUnused;
		const uint32 index = entity.GetIndex() + 1;
		const uint32 version = 0;
		m_HandlesUnused = ecs::Entity(index, version);
	}

	m_EntityChanges.Insert(entity, ecs::EntityChange());
	return entity;
}

void ecs::FrameBuffer::DestroyEntity(const ecs::Entity& entity)
{
	m_EntityChanges[entity].m_IsDestroy = true;
}