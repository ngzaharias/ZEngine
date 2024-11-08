#include "EnginePCH.h"
#include "Engine/SpriteAssetSystem.h"

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
		if (const auto* sprite = assetManager.FetchAsset<eng::SpriteAsset>(guid))
		{
			component.m_Sprite = sprite;
			if (sprite->m_Shader.IsValid())
			{
				assetManager.RequestAsset<eng::ShaderAsset>(sprite->m_Shader);
				component.m_Shader = assetManager.FetchAsset<eng::ShaderAsset>(sprite->m_Shader);
			}

			if (sprite->m_Texture2D.IsValid())
			{
				assetManager.RequestAsset<eng::Texture2DAsset>(sprite->m_Texture2D);
				component.m_Texture2D = assetManager.FetchAsset<eng::Texture2DAsset>(sprite->m_Texture2D);
			}
		}
	}

	void UnloadAsset(eng::AssetManager& assetManager, eng::SpriteAssetComponent& component)
	{
		if (component.m_Shader)
		{
			assetManager.ReleaseAsset<eng::ShaderAsset>(component.m_Shader->m_Guid);
			component.m_Shader = nullptr;
		}

		if (component.m_Sprite)
		{
			assetManager.ReleaseAsset<eng::SpriteAsset>(component.m_Sprite->m_Guid);
			component.m_Sprite = nullptr;
		}

		if (component.m_Texture2D)
		{
			assetManager.ReleaseAsset<eng::Texture2DAsset>(component.m_Texture2D->m_Guid);
			component.m_Texture2D = nullptr;
		}
	}
}

void eng::SpriteAssetSystem::Update(World& world, const GameTime& gameTime)
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

		const str::Guid& source = assetComponent.m_Sprite ? assetComponent.m_Sprite->m_Guid : str::Guid::Unassigned;
		const str::Guid& target = spriteComponent.m_Sprite;
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