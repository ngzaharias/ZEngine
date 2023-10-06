#include "GameClientPCH.h"
#include "GameClient/VisualSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/StaticMeshComponent.h>

#include "GameClient/ProjectileComponents.h"

void visual::VisualSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProjectileRequests(world);
}

void visual::VisualSystem::ProjectileRequests(World& world)
{
	const auto& changesComponent = world.GetSingleton<const projectile::ChangesComponent>();
	for (const projectile::Created& createdData : changesComponent.m_Created)
	{
		const auto& requestComponent = world.GetComponent<const projectile::CreateRequestComponent>(createdData.m_Request);
		auto& meshComponent = world.AddComponent<eng::StaticMeshComponent>(createdData.m_Projectile);
		meshComponent.m_StaticMesh = requestComponent.m_Visual.m_StaticMesh;
	}
}
