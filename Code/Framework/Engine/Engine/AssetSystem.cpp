#include "EnginePCH.h"
#include "Engine/AssetSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshComponent.h"

namespace
{
	void ProcessEntity(eng::AssetSystem::World& world, const ecs::Entity& entity)
	{
		Set<str::Guid> assets;
		if (world.HasComponent<eng::FlipbookComponent>(entity))
		{
			const auto& component = world.ReadComponent<eng::FlipbookComponent>(entity);
			assets.Add(component.m_Flipbook);
		}

		if (world.HasComponent<eng::SpriteComponent>(entity))
		{
			const auto& component = world.ReadComponent<eng::SpriteComponent>(entity);
			assets.Add(component.m_Sprite);
		}

		if (world.HasComponent<eng::StaticMeshComponent>(entity))
		{
			const auto& component = world.ReadComponent<eng::StaticMeshComponent>(entity);
			assets.Add(component.m_StaticMesh);
		}

		if (!assets.IsEmpty())
		{
			auto& component = !world.HasComponent<eng::AssetComponent>(entity)
				? world.AddComponent<eng::AssetComponent>(entity)
				: world.WriteComponent<eng::AssetComponent>(entity);

			Set<str::Guid> added, removed;
			enumerate::Difference(assets, component.m_Assets, added);
			enumerate::Difference(component.m_Assets, assets, removed);
			std::swap(assets, component.m_Assets);

			auto& manager = world.WriteResource<eng::AssetManager>();
			for (const str::Guid& guid : added)
				manager.RequestAsset(guid);

			for (const str::Guid& guid : removed)
				manager.ReleaseAsset(guid);
		}
		else if (world.HasComponent<eng::AssetComponent>(entity))
		{
			world.RemoveComponent<eng::AssetComponent>(entity);
		}
	}
}

void eng::AssetSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::FlipbookComponent>>())
		ProcessEntity(world, entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::SpriteComponent>>())
		ProcessEntity(world, entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::StaticMeshComponent>>())
		ProcessEntity(world, entity);

	for (const ecs::Entity& entity : world.Query<ecs::query::Updated<eng::FlipbookComponent>>())
		ProcessEntity(world, entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Updated<eng::SpriteComponent>>())
		ProcessEntity(world, entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Updated<eng::StaticMeshComponent>>())
		ProcessEntity(world, entity);

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::FlipbookComponent>>())
		ProcessEntity(world, entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::SpriteComponent>>())
		ProcessEntity(world, entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::StaticMeshComponent>>())
		ProcessEntity(world, entity);
}