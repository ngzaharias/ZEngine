#include "UTHelpers.h"

#include "Core/GameTime.h"

ut::RAIIHelper::RAIIHelper()
{
}

ut::RAIIHelper::~RAIIHelper()
{
	for (const ecs::Entity& entity : m_Entities)
		m_EntityWorld.DestroyEntity(entity);
	m_EntityWorld.Update({});
	m_EntityWorld.Shutdown();
}

void ut::RAIIHelper::Initialise()
{
	m_EntityWorld.Initialise();
}

void ut::RAIIHelper::Update()
{
	m_EntityWorld.Update({});
}

void ut::RAIIHelper::Update(const int32 count)
{
	for (int32 i = 0; i < count; ++i)
		m_EntityWorld.Update({});
}

bool ut::RAIIHelper::IsAlive(const ecs::Entity& entity) const
{
	return m_EntityWorld.IsAlive(entity);
}

ecs::Entity ut::RAIIHelper::CreateEntity()
{
	ecs::Entity entity = m_EntityWorld.CreateEntity();
	m_Entities.Add(entity);
	return entity;
}

void ut::RAIIHelper::DestroyEntity(const ecs::Entity& entity)
{
	m_Entities.Remove(entity);
	m_EntityWorld.DestroyEntity(entity);
}