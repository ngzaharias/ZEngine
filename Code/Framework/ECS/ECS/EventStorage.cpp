#include "ECS/EventStorage.h"

#include "Core/Profiler.h"

void ecs::EventStorage::FlushChanges()
{
	PROFILE_FUNCTION();

	for (auto&& [typeId, container] : m_BufferLocalCurr.GetAll())
		container->RemoveAll();

	for (auto&& [typeId, container] : m_BufferRemoteCurr.GetAll())
		container->RemoveAll();

	for (auto&& [typeId, rhs] : m_BufferLocalNext.GetAll())
	{
		ecs::IEventContainer& lhs = m_BufferLocalCurr.GetAt(typeId);
		rhs->MoveAll(lhs);
		rhs->RemoveAll();
	}
}
