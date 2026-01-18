#include "ECS/EventStorage.h"

#include "Core/Profiler.h"

void ecs::EventStorage::ConsumeAll(ecs::EventBuffer& buffer)
{
	PROFILE_FUNCTION();

	for (auto&& [typeId, rhs] : buffer.GetAll())
	{
		ecs::IEventContainer* lhs = m_Buffer.GetAt(typeId);
		rhs->MoveAll(*lhs);
		rhs->RemoveAll();
	}
}

void ecs::EventStorage::RemoveAll()
{
	PROFILE_FUNCTION();

	for (auto&& [typeId, container] : m_Buffer.GetAll())
	{
		container->RemoveAll();
	}
}
