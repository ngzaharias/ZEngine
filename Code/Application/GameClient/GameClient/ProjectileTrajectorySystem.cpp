#include "GameClientPCH.h"
#include "GameClient/ProjectileTrajectorySystem.h"

#include "Core/GameTime.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

#include "GameClient/MovementVelocityComponent.h"
#include "GameClient/ProjectileChangesSingleton.h"
#include "GameClient/ProjectileCreateRequestComponent.h"
#include "GameClient/ProjectileTrajectoryComponent.h"

void projectile::TrajectorySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& changesComponent = world.ReadSingleton<projectile::ChangesSingleton>();
	for (const Created& createdData : changesComponent.m_Created)
	{
		const auto& requestComponent = world.ReadComponent<projectile::CreateRequestComponent>(createdData.m_Request);
		auto& trajectoryComponent = world.AddComponent<projectile::TrajectoryComponent>(createdData.m_Projectile);
		trajectoryComponent.m_Trajectory = requestComponent.m_Trajectory.m_Trajectory;
		trajectoryComponent.m_Origin = requestComponent.m_Trajectory.m_Origin;
		trajectoryComponent.m_Scale = requestComponent.m_Trajectory.m_Scale;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<projectile::TrajectoryComponent, const movement::VelocityComponent>>())
	{
		const auto& velocityComponent = world.ReadComponent<movement::VelocityComponent>(entity);
		auto& trajectoryComponent = world.WriteComponent<projectile::TrajectoryComponent>(entity);
		trajectoryComponent.m_Distance += velocityComponent.m_Speed * gameTime.m_DeltaTime;
	}
}
