#include "EnginePCH.h"
#include "Engine/CameraPrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/CameraComponent.h"
#include "Engine/Visitor.h"

void eng::CameraPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const CameraPrototype& prototype) const
{
	world.AddComponent<eng::CameraComponent>(entity);
}

void eng::CameraPrototypeLoader::Load(eng::CameraPrototype& prototype, eng::Visitor& visitor) const
{
}