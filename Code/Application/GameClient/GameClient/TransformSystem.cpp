#include "GameClientPCH.h"
#include "GameClient/TransformSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/TransformComponent.h"
#include "GameClient/ProjectileChangesSingleton.h"
#include "GameClient/ProjectileCreateRequestComponent.h"
#include "GameClient/ProjectileTrajectoryComponent.h"
#include "Math/Quaternion.h"

void transform::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProjectileRequests(world);
}

void transform::TransformSystem::ProjectileRequests(World& world)
{
	const auto& changesComponent = world.ReadSingleton<projectile::ChangesSingleton>();
	for (const projectile::Created& createdData : changesComponent.m_Created)
	{
		const auto& requestComponent = world.ReadComponent<projectile::CreateRequestComponent>(createdData.m_Request);
		auto& transformComponent = world.AddComponent<eng::TransformComponent>(createdData.m_Projectile);
		transformComponent.m_Translate = requestComponent.m_Transform.m_Translate;
		transformComponent.m_Rotate = requestComponent.m_Transform.m_Rotate;
		transformComponent.m_Scale = requestComponent.m_Transform.m_Scale;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::TransformComponent, const projectile::TrajectoryComponent>>())
	{
		const auto& trajectoryComponent = world.ReadComponent<projectile::TrajectoryComponent>(entity);
		const float distance = trajectoryComponent.m_Distance / trajectoryComponent.m_Scale;

		auto& transformComponent = world.WriteComponent<eng::TransformComponent>(entity);
		transformComponent.m_Translate = trajectoryComponent.m_Trajectory.AtDistance(distance);
		transformComponent.m_Translate *= trajectoryComponent.m_Scale;
		transformComponent.m_Translate += trajectoryComponent.m_Origin;
	}
}