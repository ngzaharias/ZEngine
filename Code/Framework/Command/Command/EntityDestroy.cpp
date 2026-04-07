#include "Command/EntityDestroy.h"

#include "ECS/EntityWorld.h"
#include "Engine/TemplateHelpers.h"
#include "Engine/TemplateManager.h"

EntityDestroy::EntityDestroy(const str::Guid& entity)
	: m_Entity(entity)
{
}

EntityDestroy::EntityDestroy(const str::Guid& entity, str::String&& data)
	: m_Entity(entity)
	, m_Data(std::move(data))
{
}

EntityDestroy::EntityDestroy(const str::Guid& entity, const str::String& data)
	: m_Entity(entity)
	, m_Data(data)
{
}

void EntityDestroy::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Entity);
	world.DestroyEntity(entity);
}

void EntityDestroy::Redo(ecs::EntityWorld& world)
{
	Exec(world);
}

void EntityDestroy::Undo(ecs::EntityWorld& world)
{
}