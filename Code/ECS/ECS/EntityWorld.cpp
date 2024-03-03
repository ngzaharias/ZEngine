#include "ECS/EntityWorld.h"

#include <Core/Profiler.h>

#include "ECS/Manager.h"
#include "ECS/System.h"

ecs::EntityWorld::EntityWorld()
	: m_FrameBuffer()
	, m_EntityStorage()
	, m_QueryRegistry()
	, m_ManagerRegistry()
	, m_SystemRegistry()
{
}

void ecs::EntityWorld::Initialise()
{
	PROFILE_FUNCTION();

	m_SingletonEntity = CreateEntity();

	m_QueryRegistry.Initialise();

	m_ManagerRegistry.Initialise();

	// do after managers
	m_SystemRegistry.Initialise(*this);

	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);
}

void ecs::EntityWorld::Shutdown()
{
	PROFILE_FUNCTION();

	// do before managers
	m_SystemRegistry.Shutdown(*this);

	m_ManagerRegistry.Shutdown();

	DestroyEntity(m_SingletonEntity);
}

void ecs::EntityWorld::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_SystemRegistry.Update(*this, gameTime);

	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);

	// do post flush so that the entity is destroyed in the next frame
	for (const ecs::Entity& entity : m_EventEntities)
		DestroyEntity(entity);
	m_EventEntities.RemoveAll();
}
