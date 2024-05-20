#include "EnginePCH.h"
#include "Engine/StaticMeshPrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/StaticMeshComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strStaticMesh = "m_StaticMesh";
}

void eng::StaticMeshLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const StaticMeshPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::StaticMeshComponent>(entity);
	component.m_StaticMesh = prototype.m_StaticMesh;
}

void eng::StaticMeshLoader::Load(StaticMeshPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strStaticMesh, prototype.m_StaticMesh, {});
}