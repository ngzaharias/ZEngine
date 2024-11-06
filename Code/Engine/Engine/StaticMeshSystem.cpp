#include "EnginePCH.h"
#include "Engine/StaticMeshSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"

void eng::StaticMeshSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::StaticMeshComponent>>())
	{
		const auto& component = world.ReadComponent<eng::StaticMeshComponent>(entity);
		if (!component.m_StaticMesh.IsValid())
			continue;

		assetManager.RequestAsset<eng::StaticMeshAsset>(component.m_StaticMesh);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const eng::StaticMeshComponent>>())
	{
		const auto& component = world.ReadComponent<eng::StaticMeshComponent>(entity, false);
		if (!component.m_StaticMesh.IsValid())
			continue;

		assetManager.ReleaseAsset<eng::StaticMeshAsset>(component.m_StaticMesh);
	}

}