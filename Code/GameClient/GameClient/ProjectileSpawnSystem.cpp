#include "GameClientPCH.h"
#include "GameClient/ProjectileSpawnSystem.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

#include "GameClient/ProjectileComponents.h"

namespace
{
	projectile::EError VerifyDestroy(const ecs::Entity& entity, const projectile::ChangesComponent& frameData)
	{
		auto hasDuplicate = [&](const projectile::Destroyed& rhs)
		{
			return entity == rhs.m_Projectile;
		};

		if (enumerate::ContainsIf(frameData.m_Destroyed, hasDuplicate))
			return projectile::EError::ProjectileDead;

		return projectile::EError::None;
	}
}

void projectile::SpawnSystem::Initialise(World& world)
{
	world.AddSingleton<projectile::ChangesComponent>();
}

void projectile::SpawnSystem::Shutdown(World& world)
{
	world.RemoveSingleton<projectile::ChangesComponent>();
}

void projectile::SpawnSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& changesComponent = world.GetSingleton<projectile::ChangesComponent>();
	changesComponent.m_Created.RemoveAll();
	changesComponent.m_Destroyed.RemoveAll();

	ProcessLifetime(world, gameTime);
	ProcessCreate(world);
	ProcessDestroy(world);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<projectile::CreateResultComponent>>())
		world.RemoveComponent<projectile::CreateResultComponent>(entity);
}

void projectile::SpawnSystem::ProcessCreate(World& world)
{
	auto& changesComponent = world.GetSingleton<projectile::ChangesComponent>();
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<const projectile::CreateRequestComponent>>())
	{
		const auto& requestComponent = world.GetComponent<const projectile::CreateRequestComponent>(requestEntity);

		auto& resultComponent = world.AddComponent<projectile::CreateResultComponent>(requestEntity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;

		// #todo: error
		if (true)
		{
			const ecs::Entity projectileEntity = world.CreateEntity();
			auto& spawnComponent = world.AddComponent<projectile::SpawnComponent>(projectileEntity);
			spawnComponent.m_TransactionId = requestComponent.m_TransactionId;
			spawnComponent.m_Owner = requestComponent.m_Owner;
			spawnComponent.m_Timeout = requestComponent.m_Lifetime.m_Timeout;

			resultComponent.m_Projectile = projectileEntity;

			// add to frame data so we add the other components
			Created& createData = changesComponent.m_Created.Emplace();
			createData.m_Projectile = projectileEntity;
			createData.m_Request = requestEntity;
		}
	}
}

void projectile::SpawnSystem::ProcessDestroy(World& world)
{
	auto& changesComponent = world.GetSingleton<projectile::ChangesComponent>();
	for (const Destroyed& destroyData : changesComponent.m_Destroyed)
		world.DestroyEntity(destroyData.m_Projectile);
}

void projectile::SpawnSystem::ProcessLifetime(World& world, const GameTime& gameTime)
{
	auto& changesComponent = world.GetSingleton<projectile::ChangesComponent>();
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<projectile::SpawnComponent>>())
	{
		auto& spawnComponent = world.GetComponent<projectile::SpawnComponent>(entity);
		spawnComponent.m_Lifetime += gameTime.m_DeltaTime;

		if (spawnComponent.m_Lifetime >= spawnComponent.m_Timeout)
		{
			const EError error = VerifyDestroy(entity, changesComponent);
			if (error == EError::None)
			{
				Destroyed& destroyData = changesComponent.m_Destroyed.Emplace();
				destroyData.m_Projectile = entity;
			}
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const projectile::TrajectoryComponent>>())
	{
		const auto& trajectoryComponent = world.GetComponent<const projectile::TrajectoryComponent>(entity);
		if (trajectoryComponent.m_Distance >= trajectoryComponent.m_Trajectory.GetLength() * trajectoryComponent.m_Scale)
		{
			const EError error = VerifyDestroy(entity, changesComponent);
			if (error == EError::None)
			{
				Destroyed& destroyData = changesComponent.m_Destroyed.Emplace();
				destroyData.m_Projectile = entity;
			}
		}
	}
}