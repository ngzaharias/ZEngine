#include "Command/EntityCreate.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/TemplateComponent.h"
#include "Engine/TemplateHelpers.h"
#include "Engine/TemplateManager.h"

EntityCreate::EntityCreate(const str::String& name)
	: m_Guid(str::Guid::Generate())
	, m_Name(name)
{
}

void EntityCreate::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, m_Name);
	world.AddComponent<eng::TemplateComponent>(entity, m_Guid);
}

void EntityCreate::Redo(ecs::EntityWorld& world)
{
	Exec(world);
}

void EntityCreate::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	world.DestroyEntity(entity);
}