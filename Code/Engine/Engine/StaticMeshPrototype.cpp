#include "EnginePCH.h"
#include "Engine/StaticMeshPrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/StaticMeshComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::Name strStaticMesh = NAME("m_StaticMesh");
}

void eng::StaticMeshPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const StaticMeshPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::StaticMeshComponent>(entity);
	component.m_StaticMesh = prototype.m_StaticMesh;
}

void eng::StaticMeshPrototypeLoader::Load(StaticMeshPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strStaticMesh, prototype.m_StaticMesh, {});
}