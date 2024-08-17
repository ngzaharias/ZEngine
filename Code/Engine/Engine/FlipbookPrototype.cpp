#include "EnginePCH.h"
#include "Engine/FlipbookPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFlipbook = "m_Flipbook";
	const str::StringView strSize = "m_Size";
}

void eng::FlipbookLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::FlipbookPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::FlipbookComponent>(entity);
	component.m_Size = prototype.m_Size;
	component.m_Flipbook = prototype.m_Flipbook;
}

void eng::FlipbookLoader::Load(eng::FlipbookPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strSize, prototype.m_Size, Vector2u::Zero);
	visitor.Visit(strFlipbook, prototype.m_Flipbook, {});
}