#include "GameClientPCH.h"
#include "GameClient/ProjectileSpawnSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/StaticMeshComponent.h>
#include <Engine/TransformComponent.h>

#include "GameClient/ProjectileComponents.h"

void projectile::SpawnSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const projectile::RequestComponent>>())
	{
		const auto& requestComponent = world.GetComponent<const projectile::RequestComponent>(entity);

		const ecs::Entity projectileEntity = world.CreateEntity();
		auto& settingsComponent = world.AddComponent<projectile::SettingsComponent>(projectileEntity);
		settingsComponent.m_Trajectory = requestComponent.m_Trajectory.m_Trajectory;
		settingsComponent.m_Velocity = requestComponent.m_Velocity.m_Velocity;
		settingsComponent.m_Origin = requestComponent.m_Trajectory.m_Origin;
		settingsComponent.m_Lifetime = requestComponent.m_Lifetime.m_Lifetime;
		settingsComponent.m_Scale = requestComponent.m_Trajectory.m_Scale;

		auto& stateComponent = world.AddComponent<projectile::StateComponent>(projectileEntity);
		if (std::holds_alternative<speed::Constant>(requestComponent.m_Velocity.m_Velocity))
		{
			const auto& data = std::get<speed::Constant>(settingsComponent.m_Velocity);
			stateComponent.m_Velocity = data.m_Speed;
		}
		else if (std::holds_alternative<speed::Linear>(requestComponent.m_Velocity.m_Velocity))
		{
			const auto& data = std::get<speed::Linear>(settingsComponent.m_Velocity);
			stateComponent.m_Velocity = data.m_Initial;
		}

		auto& meshComponent = world.AddComponent<eng::StaticMeshComponent>(projectileEntity);
		meshComponent.m_StaticMesh = requestComponent.m_Visual.m_StaticMesh;

		auto& transformComponent = world.AddComponent<eng::TransformComponent>(projectileEntity);
		transformComponent.m_Translate = requestComponent.m_Transform.m_Translate;
		transformComponent.m_Rotate = requestComponent.m_Transform.m_Rotate;
		transformComponent.m_Scale = requestComponent.m_Transform.m_Scale;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const projectile::SettingsComponent, const projectile::StateComponent>>())
	{
		const auto& settingsComponent = world.GetComponent<const projectile::SettingsComponent>(entity);
		const auto& stateComponent = world.GetComponent<const projectile::StateComponent>(entity);

		bool hasExpired = false;
		hasExpired |= stateComponent.m_Distance >= settingsComponent.m_Trajectory.GetLength();
		hasExpired |= stateComponent.m_Lifetime >= settingsComponent.m_Lifetime;
		if (hasExpired)
			world.DestroyEntity(entity);
	}
}
