#include "GameClientPCH.h"
#include "GameClient/HiddenObjectPrototype.h"

#include <ECS/EntityWorld.h>

#include <Engine/Visitor.h>

#include "GameClient/HiddenObjectComponents.h"

namespace
{
	const str::StringView strSize = "m_Size";
	const str::StringView strSprite = "m_Sprite";
}

void hidden::ObjectPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const hidden::ObjectPrototype& prototype) const
{
	auto& component = world.AddComponent<hidden::ObjectComponent>(entity);
	component.m_Size = prototype.m_Size;
	component.m_Sprite = prototype.m_Sprite;
}

void hidden::ObjectPrototypeLoader::Load(hidden::ObjectPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strSize, prototype.m_Size, Vector2u::Zero);
	visitor.Visit(strSprite, prototype.m_Sprite, {});
}