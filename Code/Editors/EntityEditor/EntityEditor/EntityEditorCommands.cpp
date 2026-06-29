#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorCommands.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/SpriteTemplate.h"
#include "Engine/TemplateManager.h"
#include "Engine/TransformTemplate.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDHelpers.h"
#include "Engine/VisibilityTemplate.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::Name strSprite = str::Name::Create("Sprite");

	const str::StringView strUUID = "m_UUID";
}

editor::entity::EntityCreate::EntityCreate()
	: m_UUID(str::Guid::Generate())
{
}

const char* editor::entity::EntityCreate::ToString() const
{
	return "EntityCreate";
}

void editor::entity::EntityCreate::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, "Entity_");
	world.AddComponent<eng::UUIDComponent>(entity, m_UUID);
}

void editor::entity::EntityCreate::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	world.DestroyEntity(entity);
}

//////////////////////////////////////////////////////////////////////////

editor::entity::EntityCreateFromAsset::EntityCreateFromAsset(const str::Guid& uuid)
	: m_AssetUUID(uuid)
	, m_EntityUUID(str::Guid::Generate())
{
}

const char* editor::entity::EntityCreateFromAsset::ToString() const
{
	return "EntityCreateFromAsset";
}

void editor::entity::EntityCreateFromAsset::Exec(ecs::EntityWorld& world)
{
	const auto& manager = world.ReadResource<eng::AssetManager>();
	if (const eng::AssetFile* file = manager.GetAssetFile(m_AssetUUID))
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Entity_");
		world.AddComponent<eng::UUIDComponent>(entity, m_EntityUUID);

		if (file->m_Type == strSprite)
		{
			auto& spriteTemplate = world.AddComponent<eng::SpriteTemplate>(entity);
			spriteTemplate.m_Sprite = m_AssetUUID;

			auto& transformTemplate = world.AddComponent<eng::TransformTemplate>(entity);
			transformTemplate.m_Translate = Vector3f::Zero;

			auto& visibilityTemplate = world.AddComponent<eng::VisibilityTemplate>(entity);
			visibilityTemplate.m_IsVisible = true;
		}
	}
}

void editor::entity::EntityCreateFromAsset::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_EntityUUID);
	world.DestroyEntity(entity);
}

//////////////////////////////////////////////////////////////////////////

editor::entity::EntityCreateFromTemplate::EntityCreateFromTemplate(const str::Path& path)
	: m_UUID(str::Guid::Generate())
	, m_Path(path)
{
}

const char* editor::entity::EntityCreateFromTemplate::ToString() const
{
	return "EntityCreateFromTemplate";
}

void editor::entity::EntityCreateFromTemplate::Exec(ecs::EntityWorld& world)
{
	if (m_Path.IsEmpty())
	{
		const ecs::Entity entity = world.CreateEntity();
		world.AddComponent<ecs::NameComponent>(entity, "Entity_");
		world.AddComponent<eng::UUIDComponent>(entity, m_UUID);
	}
	else
	{
		Visitor visitor;
		if (visitor.LoadFromFile(m_Path))
		{
			// #hack: override the UUID since we can't access the component after it is added
			visitor.Write(strUUID, m_UUID);

			const ecs::Entity entity = world.CreateEntity();
			const auto& manager = world.ReadResource<eng::TemplateManager>();
			manager.WriteEntity(world, entity, visitor);
		}
	}
}

void editor::entity::EntityCreateFromTemplate::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	world.DestroyEntity(entity);
}

//////////////////////////////////////////////////////////////////////////

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

const char* editor::entity::EntityDestroy::ToString() const
{
	return "EntityDestroy";
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