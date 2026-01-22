#include "ECS/EventStorage.h"

#include "Core/Profiler.h"

void ecs::EventStorage::FlushChanges()
{
	PROFILE_FUNCTION();

	for (auto&& [typeId, container] : m_MainBufferCurr.GetAll())
		container->RemoveAll();

	for (auto&& [typeId, container] : m_SyncBufferCurr.GetAll())
		container->RemoveAll();

	for (auto&& [typeId, rhs] : m_MainBufferNext.GetAll())
	{
		ecs::IEventContainer& lhs = m_MainBufferCurr.GetAt(typeId);
		rhs->MoveAll(lhs);
		rhs->RemoveAll();
	}
}
