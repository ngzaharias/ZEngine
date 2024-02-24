#include "GameClientPCH.h"
#include "GameClient/SpringDamperSystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/TransformComponent.h>

#include "GameClient/SpringComponents.h"

void spring::DamperSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, spring::DamperComponent>>())
	{
		auto& transform = world.GetComponent<eng::TransformComponent>(entity);
		auto& damper = world.GetComponent<spring::DamperComponent>(entity);
	}
}