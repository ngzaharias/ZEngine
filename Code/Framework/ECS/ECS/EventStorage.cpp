#include "ECS/EventStorage.h"

#include "Core/Profiler.h"

auto ecs::EventStorage::GetMainBuffer() -> ecs::EventBuffer&
{
	return m_MainBuffer;
}

auto ecs::EventStorage::GetMainBuffer() const -> const ecs::EventBuffer&
{
	return m_MainBuffer;
}

auto ecs::EventStorage::GetNextBuffer() -> ecs::EventBuffer&
{
	return m_NextBuffer;
}

auto ecs::EventStorage::GetNextBuffer() const -> const ecs::EventBuffer&
{
	return m_NextBuffer;
}

auto ecs::EventStorage::GetSyncBuffer() -> ecs::EventBuffer&
{
	return m_SyncBuffer;
}

auto ecs::EventStorage::GetSyncBuffer() const -> const ecs::EventBuffer&
{
	return m_SyncBuffer;
}

void ecs::EventStorage::FlushChanges()
{
	PROFILE_FUNCTION();

	for (auto&& [typeId, container] : m_MainBuffer.GetAll())
		container->RemoveAll();

	for (auto&& [typeId, container] : m_SyncBuffer.GetAll())
		container->RemoveAll();

	for (auto&& [typeId, rhs] : m_NextBuffer.GetAll())
	{
		ecs::IEventContainer& lhs = m_MainBuffer.GetAt(typeId);
		rhs->MoveAll(lhs);
		rhs->RemoveAll();
	}
}