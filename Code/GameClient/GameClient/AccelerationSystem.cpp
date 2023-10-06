#include "GameClientPCH.h"
#include "GameClient/AccelerationSystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/MovementComponents.h"
#include "GameClient/ProjectileComponents.h"

void movement::AccelerationSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& changesComponent = world.GetSingleton<const projectile::ChangesComponent>();
	for (const projectile::Created& createdData : changesComponent.m_Created)
	{
		const auto& requestComponent = world.GetComponent<const projectile::CreateRequestComponent>(createdData.m_Request);
		if (requestComponent.m_Velocity.m_Acceleration != 0.f)
		{
			auto& accelerationComponent = world.AddComponent<movement::AccelerationComponent>(createdData.m_Projectile);
			accelerationComponent.m_Acceleration = requestComponent.m_Velocity.m_Acceleration;
			accelerationComponent.m_Maximum = requestComponent.m_Velocity.m_Maximum;
			accelerationComponent.m_Minimum = requestComponent.m_Velocity.m_Minimum;
		}
	}
}
