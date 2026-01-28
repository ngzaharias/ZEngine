#include "ECS/SingletonStorage.h"

#include "Core/Profiler.h"

ecs::ISingletonContainer& ecs::SingletonStorage::GetAt(const ecs::SingletonId typeId)
{
	return *m_Containers.Get(typeId);
}

const ecs::ISingletonContainer& ecs::SingletonStorage::GetAt(const ecs::SingletonId typeId) const
{
	return *m_Containers.Get(typeId);
}

void ecs::SingletonStorage::FlushChanges()
{
	PROFILE_FUNCTION();

	std::swap(m_UpdatedCurr, m_UpdatedNext);
	m_UpdatedNext.RemoveAll();
}
