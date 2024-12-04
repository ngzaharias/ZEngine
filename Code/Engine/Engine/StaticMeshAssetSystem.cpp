#include "EnginePCH.h"
#include "Engine/StaticMeshAssetSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"

namespace
{
	void LoadAsset(eng::AssetManager& assetManager, eng::StaticMeshAssetComponent& component, const str::Guid& guid)
	{
		if (!guid.IsValid())
			return;

		assetManager.RequestAsset<eng::StaticMeshAsset>(guid);
		component.m_Asset = assetManager.FetchAsset<eng::StaticMeshAsset>(guid);
	}

	void UnloadAsset(eng::AssetManager& assetManager, eng::StaticMeshAssetComponent& component)
	{
		if (component.m_Asset)
		{
			assetManager.ReleaseAsset<eng::StaticMeshAsset>(component.m_Asset->m_Guid);
			component.m_Asset = nullptr;
		}
	}
}

void eng::StaticMeshAssetSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	using AddedQuery = ecs::query
		::Include<eng::StaticMeshComponent>
		::Exclude<eng::StaticMeshAssetComponent>;
	for (const ecs::Entity& entity : world.Query<AddedQuery>())
	{
		const auto& meshComponent = world.ReadComponent<eng::StaticMeshComponent>(entity);
		auto& assetComponent = world.AddComponent<eng::StaticMeshAssetComponent>(entity);
		LoadAsset(assetManager, assetComponent, meshComponent.m_StaticMesh);
	}

	using UpdatedQuery = ecs::query
		::Updated<eng::StaticMeshComponent>
		::Include<eng::StaticMeshAssetComponent>;
	for (const ecs::Entity& entity : world.Query<UpdatedQuery>())
	{
		const auto& meshComponent = world.ReadComponent<eng::StaticMeshComponent>(entity);
		auto& assetComponent = world.WriteComponent<eng::StaticMeshAssetComponent>(entity);

		const str::Guid& source = assetComponent.m_Asset ? assetComponent.m_Asset->m_Guid : str::Guid::Unassigned;
		const str::Guid& target = meshComponent.m_StaticMesh;
		if (source != target)
		{
			UnloadAsset(assetManager, assetComponent);
			LoadAsset(assetManager, assetComponent, meshComponent.m_StaticMesh);
		}
	}

	using RemovedQuery = ecs::query
		::Exclude<eng::StaticMeshComponent>
		::Include<eng::StaticMeshAssetComponent>;
	for (const ecs::Entity& entity : world.Query<RemovedQuery>())
	{
		auto& assetComponent = world.WriteComponent<eng::StaticMeshAssetComponent>(entity);
		UnloadAsset(assetManager, assetComponent);

		world.RemoveComponent<eng::StaticMeshAssetComponent>(entity);
	}
}