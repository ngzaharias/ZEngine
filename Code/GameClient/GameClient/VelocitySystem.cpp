#include "GameClientPCH.h"
#include "GameClient/VelocitySystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/MovementComponents.h"

void movement::VelocitySystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<movement::VelocityComponent, const movement::AccelerationComponent>>())
	{
		const auto& accelerationComponent = world.GetComponent<const movement::AccelerationComponent>(entity);
		auto& velocityComponent = world.GetComponent<movement::VelocityComponent>(entity);

		velocityComponent.m_Speed += accelerationComponent.m_Speed * gameTime.m_DeltaTime;
		velocityComponent.m_Speed = math::Clamp(velocityComponent.m_Speed, accelerationComponent.m_Minimum, accelerationComponent.m_Maximum);
	}
}