#include "GameClientPCH.h"
#include "GameClient/VelocitySystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/MovementAccelerationComponent.h"
#include "GameClient/MovementVelocityComponent.h"
#include "GameClient/ProjectileChangesSingleton.h"
#include "GameClient/ProjectileCreateRequestComponent.h"

void movement::VelocitySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& changesComponent = world.ReadSingleton<projectile::ChangesSingleton>();
	for (const projectile::Created& createdData : changesComponent.m_Created)
	{
		const auto& requestComponent = world.ReadComponent<projectile::CreateRequestComponent>(createdData.m_Request);
		auto& velocityComponent = world.AddComponent<movement::VelocityComponent>(createdData.m_Projectile);
		velocityComponent.m_Speed = requestComponent.m_Velocity.m_Initial;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<movement::VelocityComponent, const movement::AccelerationComponent>>())
	{
		const auto& accelerationComponent = world.ReadComponent<movement::AccelerationComponent>(entity);
		auto& velocityComponent = world.WriteComponent<movement::VelocityComponent>(entity);
		velocityComponent.m_Speed += accelerationComponent.m_Acceleration * gameTime.m_DeltaTime;
		velocityComponent.m_Speed = math::Clamp(velocityComponent.m_Speed, accelerationComponent.m_Minimum, accelerationComponent.m_Maximum);
	}
}
