#include "ECS/EntityWorld.h"

#include "ECS/Manager.h"
#include "ECS/System.h"

ecs::EntityWorld::EntityWorld()
	: m_FrameBuffer()
	, m_EntityStorage()
	, m_QueryRegistry()
	, m_ManagerRegistry()
	, m_SystemRegistry(*this)
{
}

ecs::EntityWorld::~EntityWorld()
{
}

bool ecs::EntityWorld::IsInitialised() const
{
	return m_IsInitialised;
}

void ecs::EntityWorld::Initialise()
{
	m_IsInitialised = true;
	m_SingletonEntity = CreateEntity();

	m_QueryRegistry.Initialise();

	m_ManagerRegistry.Initialise();

	// do after managers
	m_SystemRegistry.Initialise();

	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);
}

void ecs::EntityWorld::Shutdown()
{
	// do before managers
	m_SystemRegistry.Shutdown();

	m_ManagerRegistry.Shutdown();

	DestroyEntity(m_SingletonEntity);
}

void ecs::EntityWorld::Update(const GameTime& gameTime)
{
	m_SystemRegistry.Update(gameTime);

	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);

	// do post flush so that the entity is destroyed in the next frame
	for (const ecs::Entity& entity : m_EventEntities)
		DestroyEntity(entity);
	m_EventEntities.RemoveAll();
}
