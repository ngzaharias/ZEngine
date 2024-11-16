#include "GameClientPCH.h"
#include "GameClient/HexmapAssetSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteAsset.h"
#include "Engine/Texture2DAsset.h"
#include "GameClient/HexmapAssetComponent.h"
#include "GameClient/HexmapFragmentComponent.h"

namespace
{
	void LoadAsset(eng::AssetManager& assetManager, hexmap::AssetComponent& component, const str::Guid& guid)
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

	void UnloadAsset(eng::AssetManager& assetManager, hexmap::AssetComponent& component)
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

void hexmap::AssetSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	using AddedQuery = ecs::query
		::Include<hexmap::FragmentComponent>
		::Exclude<hexmap::AssetComponent>;
	for (const ecs::Entity& entity : world.Query<AddedQuery>())
	{
		const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
		auto& assets = world.AddComponent<hexmap::AssetComponent>(entity);
		LoadAsset(assetManager, assets, fragment.m_Sprite);
	}

	using UpdatedQuery = ecs::query
		::Updated<hexmap::FragmentComponent>
		::Include<hexmap::AssetComponent>;
	for (const ecs::Entity& entity : world.Query<UpdatedQuery>())
	{
		const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(entity);
		auto& assets = world.WriteComponent<hexmap::AssetComponent>(entity);

		const str::Guid& source = assets.m_Sprite ? assets.m_Sprite->m_Guid : str::Guid::Unassigned;
		const str::Guid& target = fragment.m_Sprite;
		if (source != target)
		{
			UnloadAsset(assetManager, assets);
			LoadAsset(assetManager, assets, fragment.m_Sprite);
		}
	}

	using RemovedQuery = ecs::query
		::Exclude<hexmap::FragmentComponent>
		::Include<hexmap::AssetComponent>;
	for (const ecs::Entity& entity : world.Query<RemovedQuery>())
	{
		auto& assets = world.WriteComponent<hexmap::AssetComponent>(entity);
		UnloadAsset(assetManager, assets);

		world.RemoveComponent<hexmap::AssetComponent>(entity);
	}
}