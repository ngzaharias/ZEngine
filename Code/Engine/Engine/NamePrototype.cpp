#include "EnginePCH.h"
#include "Engine/NamePrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/NameComponent.h"
#include "Engine/Visitor.h"

void eng::NamePrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const NamePrototype& prototype) const
{
	auto& component = world.AddComponent<eng::NameComponent>(entity);
	component.m_Name = prototype.m_Name;
}

void eng::NamePrototypeLoader::Load(NamePrototype& prototype, eng::Visitor& visitor) const
{
	// do nothing, we can use the name from the asset
}