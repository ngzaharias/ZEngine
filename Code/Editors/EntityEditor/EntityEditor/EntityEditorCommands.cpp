#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorCommands.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/TemplateComponent.h"
#include "Engine/TemplateHelpers.h"
#include "Engine/TemplateManager.h"

editor::entity::EntityCreate::EntityCreate(const str::String& name)
	: m_Guid(str::Guid::Generate())
	, m_Name(name)
{
}

void editor::entity::EntityCreate::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, m_Name);
	world.AddComponent<eng::TemplateComponent>(entity, m_Guid);
}

void editor::entity::EntityCreate::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	world.DestroyEntity(entity);
}

editor::entity::EntityDestroy::EntityDestroy(const str::Guid& entity)
	: m_Entity(entity)
{
}

editor::entity::EntityDestroy::EntityDestroy(const str::Guid& entity, str::String&& data)
	: m_Entity(entity)
	, m_Data(std::move(data))
{
}

editor::entity::EntityDestroy::EntityDestroy(const str::Guid& entity, const str::String& data)
	: m_Entity(entity)
	, m_Data(data)
{
}

void editor::entity::EntityDestroy::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Entity);
	world.DestroyEntity(entity);
}

void editor::entity::EntityDestroy::Undo(ecs::EntityWorld& world)
{
}