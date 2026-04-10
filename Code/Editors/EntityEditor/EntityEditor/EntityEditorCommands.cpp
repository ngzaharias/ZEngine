#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorCommands.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/TemplateManager.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDHelpers.h"

editor::entity::EntityCreate::EntityCreate(const str::Guid& uuid, const str::StringView& name)
	: m_UUID(str::Guid::Generate())
	, m_Name(name)
{
}

void editor::entity::EntityCreate::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, m_Name);
	world.AddComponent<eng::UUIDComponent>(entity, m_UUID);
}

void editor::entity::EntityCreate::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	world.DestroyEntity(entity);
}

editor::entity::EntityDestroy::EntityDestroy(const str::Guid& uuid)
	: m_UUID(uuid)
{
}

editor::entity::EntityDestroy::EntityDestroy(const str::Guid& uuid, str::String&& data)
	: m_UUID(uuid)
	, m_Data(std::move(data))
{
}

editor::entity::EntityDestroy::EntityDestroy(const str::Guid& uuid, const str::String& data)
	: m_UUID(uuid)
	, m_Data(data)
{
}

void editor::entity::EntityDestroy::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	world.DestroyEntity(entity);
}

void editor::entity::EntityDestroy::Undo(ecs::EntityWorld& world)
{
	Visitor visitor(m_Data);
	const ecs::Entity entity = world.CreateEntity();

	const auto& manager = world.ReadResource<eng::TemplateManager>();
	manager.WriteEntity(world, entity, visitor);
}