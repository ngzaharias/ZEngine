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
	void LoadAsset(eng::AssetManager& assetManager, const str::Guid& guid)
	{
		if (!guid.IsValid())
			return;

		assetManager.RequestAsset<eng::SpriteAsset>(guid);
		if (const auto* sprite = assetManager.FetchAsset<eng::SpriteAsset>(guid))
		{
			if (sprite->m_Shader.IsValid())
				assetManager.RequestAsset<eng::ShaderAsset>(sprite->m_Shader);
			if (sprite->m_Texture2D.IsValid())
				assetManager.RequestAsset<eng::Texture2DAsset>(sprite->m_Texture2D);
		}
	}

	void UnloadAsset(eng::AssetManager& assetManager, const str::Guid& guid)
	{
		if (!guid.IsValid())
			return;

		if (const auto* sprite = assetManager.FetchAsset<eng::SpriteAsset>(guid))
		{
			if (sprite->m_Shader.IsValid())
				assetManager.ReleaseAsset<eng::ShaderAsset>(sprite->m_Shader);
			if (sprite->m_Texture2D.IsValid())
				assetManager.ReleaseAsset<eng::Texture2DAsset>(sprite->m_Texture2D);
		}
		assetManager.ReleaseAsset<eng::SpriteAsset>(guid);
	}
}

void eng::SpriteSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::SpriteComponent>>())
	{
		const auto& component = world.ReadComponent<eng::SpriteComponent>(entity);
		LoadAsset(assetManager, component.m_Sprite);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::SpriteRequestComponent>>())
	{
		const auto& request = world.ReadComponent<eng::SpriteRequestComponent>(entity);
		if (!world.IsAlive(request.m_Entity))
			continue;
		if (!world.HasComponent<eng::SpriteComponent>(request.m_Entity))
			continue;

		auto& component = world.WriteComponent<eng::SpriteComponent>(request.m_Entity);
		if (request.m_Sprite)
		{
			UnloadAsset(assetManager, component.m_Sprite);
			component.m_Sprite = *request.m_Sprite;
			LoadAsset(assetManager, component.m_Sprite);
		}

		if (request.m_Colour)
			component.m_Colour = *request.m_Colour;

		if (request.m_Size)
			component.m_Size = *request.m_Size;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const eng::SpriteComponent>>())
	{
		const auto& component = world.ReadComponent<eng::SpriteComponent>(entity, false);
		UnloadAsset(assetManager, component.m_Sprite);
	}
}