#include "ECS/EventBuffer.h"

ecs::IEventContainer* ecs::EventBuffer::GetAt(const ecs::EventId typeId)
{
	return m_Containers.Get(typeId);
}

const ecs::IEventContainer* ecs::EventBuffer::GetAt(const ecs::EventId typeId) const
{
	return m_Containers.Get(typeId);
}

ecs::EventBuffer::Containers& ecs::EventBuffer::GetAll()
{
	return m_Containers;
}

const ecs::EventBuffer::Containers& ecs::EventBuffer::GetAll() const
{
	return m_Containers;
}