#include "EnginePCH.h"
#include "Engine/SpritePrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/SpriteComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strSize = NAME("m_Size");
	const str::Name strSprite = NAME("m_Sprite");
}

void eng::SpritePrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const SpritePrototype& prototype) const
{
	auto& component = world.AddComponent<eng::SpriteComponent>(entity);
	component.m_Size = prototype.m_Size;
	component.m_Sprite = prototype.m_Sprite;
}

void eng::SpritePrototypeLoader::Load(SpritePrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strSize, prototype.m_Size, Vector2u::Zero);
	visitor.Visit(strSprite, prototype.m_Sprite, {});
}