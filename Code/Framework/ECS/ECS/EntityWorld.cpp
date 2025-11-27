#include "ECS/EntityWorld.h"

#include "Core/Profiler.h"
#include "ECS/NameComponent.h"
#include "ECS/System.h"

ecs::EntityWorld::EntityWorld()
	: m_FrameBuffer()
	, m_EntityStorage()
	, m_QueryRegistry()
	, m_SystemRegistry()
{
	RegisterComponent<ecs::NameComponent>();
}

void ecs::EntityWorld::Initialise()
{
	PROFILE_FUNCTION();

	m_QueryRegistry.Initialise();
	m_SystemRegistry.Initialise(*this);

	// flush the initialise
	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);
}

void ecs::EntityWorld::Shutdown()
{
	PROFILE_FUNCTION();

	m_SystemRegistry.Shutdown(*this);
}

void ecs::EntityWorld::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_SystemRegistry.Update(*this, gameTime);

	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);
}