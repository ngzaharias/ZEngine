#include "EnginePCH.h"
#include "Engine/SpriteSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteComponent.h"
#include "Engine/SpriteAsset.h"
#include "Engine/Texture2DAsset.h"

namespace
{
	void LoadAsset(eng::AssetManager& assetManager, eng::SpriteAssetComponent& component, const str::Guid& guid)
	{
		if (!guid.IsValid())
			return;

		assetManager.RequestAsset<eng::SpriteAsset>(guid);
		if (const auto* asset = assetManager.FetchAsset<eng::SpriteAsset>(guid))
		{
			if (asset->m_Shader.IsValid())
				assetManager.RequestAsset<eng::ShaderAsset>(asset->m_Shader);
			if (asset->m_Texture2D.IsValid())
				assetManager.RequestAsset<eng::Texture2DAsset>(asset->m_Texture2D);

			component.m_Asset = asset;
		}
	}

	void UnloadAsset(eng::AssetManager& assetManager, eng::SpriteAssetComponent& component)
	{
		if (const eng::SpriteAsset* asset = component.m_Asset)
		{
			if (asset->m_Shader.IsValid())
				assetManager.ReleaseAsset<eng::ShaderAsset>(asset->m_Shader);
			if (asset->m_Texture2D.IsValid())
				assetManager.ReleaseAsset<eng::Texture2DAsset>(asset->m_Texture2D);
			assetManager.ReleaseAsset<eng::SpriteAsset>(asset->m_Guid);

			component.m_Asset = nullptr;
		}
	}
}

void eng::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	using AddedQuery = ecs::query
		::Added<eng::SpriteComponent>
		::Exclude<eng::SpriteAssetComponent>;
	for (const ecs::Entity& entity : world.Query<AddedQuery>())
	{
		const auto& spriteComponent = world.ReadComponent<eng::SpriteComponent>(entity);
		auto& assetComponent = world.AddComponent<eng::SpriteAssetComponent>(entity);
		LoadAsset(assetManager, assetComponent, spriteComponent.m_Sprite);
	}

	using UpdatedQuery = ecs::query
		::Updated<eng::SpriteComponent>
		::Include<eng::SpriteAssetComponent>;
	for (const ecs::Entity& entity : world.Query<UpdatedQuery>())
	{
		const auto& spriteComponent = world.ReadComponent<eng::SpriteComponent>(entity);
		auto& assetComponent = world.WriteComponent<eng::SpriteAssetComponent>(entity);

		const str::Guid source = assetComponent.m_Asset ? assetComponent.m_Asset->m_Guid : str::Guid::Unassigned;
		const str::Guid target = spriteComponent.m_Sprite;
		if (source != target)
		{
			UnloadAsset(assetManager, assetComponent);
			LoadAsset(assetManager, assetComponent, spriteComponent.m_Sprite);
		}
	}

	using RemovedQuery = ecs::query
		::Removed<eng::SpriteComponent>
		::Include<eng::SpriteAssetComponent>;
	for (const ecs::Entity& entity : world.Query<RemovedQuery>())
	{
		auto& assetComponent = world.WriteComponent<eng::SpriteAssetComponent>(entity);
		UnloadAsset(assetManager, assetComponent);
	}
}