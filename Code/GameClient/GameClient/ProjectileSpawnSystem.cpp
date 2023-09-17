#include "GameClientPCH.h"
#include "GameClient/ProjectileSpawnSystem.h"

#include <Core/GameTime.h>
#include <Core/Guid.h>
#include <Core/Quaternion.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/LinesComponent.h>
#include <Engine/StaticMeshComponent.h>
#include <Engine/TransformComponent.h>

#include "GameClient/MovementComponents.h"
#include "GameClient/ProjectileComponents.h"

namespace
{
	const str::Guid strStaticMesh = str::Guid::Create("d0284c56-3e6a-49a2-9a80-25ff2731a3de");
}

void projectile::SpawnSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const projectile::SpawnRequestComponent>>())
	{
		const auto& requestComponent = world.GetComponent<const projectile::SpawnRequestComponent>(entity);

		const ecs::Entity projectileEntity = world.CreateEntity();
		auto& lifetimeComponent = world.AddComponent<projectile::LifetimeComponent>(projectileEntity);
		lifetimeComponent.m_Lifetime = requestComponent.m_Lifetime;

		auto& trajectoryComponent = world.AddComponent<projectile::TrajectoryComponent>(projectileEntity);
		trajectoryComponent.m_Translate = requestComponent.m_Translate;
		trajectoryComponent.m_Trajectory = requestComponent.m_Trajectory;

		auto& transformComponent = world.AddComponent<eng::TransformComponent>(projectileEntity);
		transformComponent.m_Translate = requestComponent.m_Translate;
		transformComponent.m_Rotate = requestComponent.m_Rotate;
		transformComponent.m_Scale = requestComponent.m_Scale;

		if (std::holds_alternative<speed::Constant>(requestComponent.m_Velocity))
		{
			const speed::Constant& velocity = std::get<speed::Constant>(requestComponent.m_Velocity);

			auto& velocityComponent = world.AddComponent<movement::VelocityComponent>(projectileEntity);
			velocityComponent.m_Speed = velocity.m_Speed;
		}
		else if (std::holds_alternative<speed::Linear>(requestComponent.m_Velocity))
		{
			const speed::Linear& velocity = std::get<speed::Linear>(requestComponent.m_Velocity);

			auto& accelerationComponent = world.AddComponent<movement::AccelerationComponent>(projectileEntity);
			accelerationComponent.m_Maximum = velocity.m_Maximum;
			accelerationComponent.m_Minimum = velocity.m_Minimum;
			accelerationComponent.m_Speed = velocity.m_Acceleration;

			auto& velocityComponent = world.AddComponent<movement::VelocityComponent>(projectileEntity);
			velocityComponent.m_Speed = velocity.m_Initial;
		}

		auto& meshComponent = world.AddComponent<eng::StaticMeshComponent>(projectileEntity);
		meshComponent.m_StaticMesh = strStaticMesh;
	}


	auto& linesComponent = world.GetSingleton<eng::LinesComponent>();
	using Query = ecs::query::Include<
		eng::TransformComponent,
		projectile::TrajectoryComponent,
		const movement::VelocityComponent>;
	for (const ecs::Entity& entity : world.Query<Query>())
	{
		const auto& velocityComponent = world.GetComponent<const movement::VelocityComponent>(entity);
		auto& transformComponent = world.GetComponent<eng::TransformComponent>(entity);
		auto& projectileComponent = world.GetComponent<projectile::TrajectoryComponent>(entity);

		// #todo: scale and rotate
		const Vector3f position = projectileComponent.m_Trajectory.AtDistance(projectileComponent.m_Distance);

		projectileComponent.m_Distance += velocityComponent.m_Speed * gameTime.m_DeltaTime;
		transformComponent.m_Translate = projectileComponent.m_Translate + position;
		if (projectileComponent.m_Distance > projectileComponent.m_Trajectory.GetLength())
			world.DestroyEntity(entity);
	}
}
