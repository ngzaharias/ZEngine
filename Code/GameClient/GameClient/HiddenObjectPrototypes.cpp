#include "GameClientPCH.h"
#include "GameClient/HiddenObjectPrototypes.h"

#include "ECS/EntityWorld.h"
#include "Engine/Visitor.h"
#include "GameClient/HiddenObjectComponents.h"

namespace
{
	const str::StringView strGroup = "m_Group";
	const str::StringView strObjects = "m_Objects";
	const str::StringView strSize = "m_Size";
	const str::StringView strSprite = "m_Sprite";
}

void hidden::ObjectLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const hidden::ObjectPrototype& prototype) const
{
	auto& component = world.AddComponent<hidden::ObjectComponent>(entity);
	component.m_Size = prototype.m_Size;
	component.m_Group = prototype.m_Group;
	component.m_Sprite = prototype.m_Sprite;
}

void hidden::ObjectLoader::Load(hidden::ObjectPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strGroup, prototype.m_Group, {});
	visitor.Visit(strSize, prototype.m_Size, Vector2u::Zero);
	visitor.Visit(strSprite, prototype.m_Sprite, {});
}

void hidden::GroupLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const hidden::GroupPrototype& prototype) const
{
	auto& groupComponent = world.AddComponent<hidden::GroupComponent>(entity);
	for (const str::Guid& guid : prototype.m_Objects)
		groupComponent.m_Objects.Add(guid);
	groupComponent.m_Size = prototype.m_Size;
	groupComponent.m_Sprite = prototype.m_Sprite;
}

void hidden::GroupLoader::Load(hidden::GroupPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strObjects, prototype.m_Objects, {});
	visitor.Visit(strSize, prototype.m_Size, {});
	visitor.Visit(strSprite, prototype.m_Sprite, {});
}