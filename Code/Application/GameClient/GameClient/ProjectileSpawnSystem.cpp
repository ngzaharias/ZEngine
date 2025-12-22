#include "GameClientPCH.h"
#include "GameClient/ProjectileSpawnSystem.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/ProjectileChangesSingleton.h"
#include "GameClient/ProjectileCreateRequestComponent.h"
#include "GameClient/ProjectileCreateResultComponent.h"
#include "GameClient/ProjectileSpawnComponent.h"
#include "GameClient/ProjectileTrajectoryComponent.h"

namespace
{
	projectile::EError VerifyDestroy(const ecs::Entity& entity, const projectile::ChangesSingleton& frameData)
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

void projectile::SpawnSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& changesComponent = world.WriteSingleton<projectile::ChangesSingleton>();
	changesComponent.m_Created.RemoveAll();
	changesComponent.m_Destroyed.RemoveAll();

	ProcessLifetime(world, gameTime);
	ProcessCreate(world);
	ProcessDestroy(world);

	for (auto&& view : world.Query<ecs::query::Include<projectile::CreateResultComponent>>())
		world.RemoveComponent<projectile::CreateResultComponent>(view);
}

void projectile::SpawnSystem::ProcessCreate(World& world)
{
	auto& changesComponent = world.WriteSingleton<projectile::ChangesSingleton>();

	using Query = ecs::query
		::Added<const projectile::CreateRequestComponent>
		::Include<const projectile::CreateRequestComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const auto& requestComponent = view.ReadRequired<projectile::CreateRequestComponent>();

		auto& resultComponent = world.AddComponent<projectile::CreateResultComponent>(view);
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
			createData.m_Request = view;
		}
	}
}

void projectile::SpawnSystem::ProcessDestroy(World& world)
{
	auto& changesComponent = world.WriteSingleton<projectile::ChangesSingleton>();
	for (const Destroyed& destroyData : changesComponent.m_Destroyed)
		world.DestroyEntity(destroyData.m_Projectile);
}

void projectile::SpawnSystem::ProcessLifetime(World& world, const GameTime& gameTime)
{
	auto& changesComponent = world.WriteSingleton<projectile::ChangesSingleton>();
	for (auto&& view : world.Query<ecs::query::Include<projectile::SpawnComponent>>())
	{
		auto& spawnComponent = view.WriteRequired<projectile::SpawnComponent>();
		spawnComponent.m_Lifetime += gameTime.m_DeltaTime;

		if (spawnComponent.m_Lifetime >= spawnComponent.m_Timeout)
		{
			const EError error = VerifyDestroy(view, changesComponent);
			if (error == EError::None)
			{
				Destroyed& destroyData = changesComponent.m_Destroyed.Emplace();
				destroyData.m_Projectile = view;
			}
		}
	}

	for (auto&& view : world.Query<ecs::query::Include<const projectile::TrajectoryComponent>>())
	{
		const auto& trajectoryComponent = view.ReadRequired<projectile::TrajectoryComponent>();
		if (trajectoryComponent.m_Distance >= trajectoryComponent.m_Trajectory.GetLength() * trajectoryComponent.m_Scale)
		{
			const EError error = VerifyDestroy(view, changesComponent);
			if (error == EError::None)
			{
				Destroyed& destroyData = changesComponent.m_Destroyed.Emplace();
				destroyData.m_Projectile = view;
			}
		}
	}
}