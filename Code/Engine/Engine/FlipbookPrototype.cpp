#include "EnginePCH.h"
#include "Engine/FlipbookPrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/FlipbookComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strFlipbook = NAME("m_Flipbook");
	const str::Name strSize = NAME("m_Size");
}

void eng::FlipbookPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const FlipbookPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::FlipbookComponent>(entity);
	component.m_Size = prototype.m_Size;
	component.m_Flipbook = prototype.m_Flipbook;
}

void eng::FlipbookPrototypeLoader::Load(FlipbookPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strSize, prototype.m_Size, Vector2u::Zero);
	visitor.Visit(strFlipbook, prototype.m_Flipbook, {});
}