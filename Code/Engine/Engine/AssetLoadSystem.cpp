#include "EnginePCH.h"
#include "Engine/AssetLoadSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteComponent.h"
#include "Engine/SpriteAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/Texture2DAsset.h"

void eng::AssetLoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::FlipbookComponent>>())
	{
		const auto& component = world.ReadComponent<eng::FlipbookComponent>(entity);
		if (!component.m_Flipbook.IsValid())
			continue;

		assetManager.RequestAsset<eng::FlipbookAsset>(component.m_Flipbook);
		if (const auto* flipbook = assetManager.FetchAsset<eng::FlipbookAsset>(component.m_Flipbook))
		{
			if (flipbook->m_Shader.IsValid())
				assetManager.RequestAsset<eng::ShaderAsset>(flipbook->m_Shader);
			if (flipbook->m_Texture2D.IsValid())
				assetManager.RequestAsset<eng::Texture2DAsset>(flipbook->m_Texture2D);
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::SpriteComponent>>())
	{
		const auto& component = world.ReadComponent<eng::SpriteComponent>(entity);
		if (!component.m_Sprite.IsValid())
			continue;

		assetManager.RequestAsset<eng::SpriteAsset>(component.m_Sprite);
		if (const auto* sprite = assetManager.FetchAsset<eng::SpriteAsset>(component.m_Sprite))
		{
			if (sprite->m_Shader.IsValid())
				assetManager.RequestAsset<eng::ShaderAsset>(sprite->m_Shader);
			if (sprite->m_Texture2D.IsValid())
				assetManager.RequestAsset<eng::Texture2DAsset>(sprite->m_Texture2D);
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::StaticMeshComponent>>())
	{
		const auto& component = world.ReadComponent<eng::StaticMeshComponent>(entity);
		if (!component.m_StaticMesh.IsValid())
			continue;

		assetManager.RequestAsset<eng::StaticMeshAsset>(component.m_StaticMesh);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const eng::FlipbookComponent>>())
	{
		const auto& component = world.ReadComponent<eng::FlipbookComponent>(entity, false);
		if (!component.m_Flipbook.IsValid())
			continue;

		if (const auto* flipbook = assetManager.FetchAsset<eng::FlipbookAsset>(component.m_Flipbook))
		{
			if (flipbook->m_Shader.IsValid())
				assetManager.ReleaseAsset<eng::ShaderAsset>(flipbook->m_Shader);
			if (flipbook->m_Texture2D.IsValid())
				assetManager.ReleaseAsset<eng::Texture2DAsset>(flipbook->m_Texture2D);
		}
		assetManager.ReleaseAsset<eng::FlipbookAsset>(component.m_Flipbook);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const eng::SpriteComponent>>())
	{
		const auto& component = world.ReadComponent<eng::SpriteComponent>(entity, false);
		if (!component.m_Sprite.IsValid())
			continue;

		if (const auto* sprite = assetManager.FetchAsset<eng::SpriteAsset>(component.m_Sprite))
		{
			if (sprite->m_Shader.IsValid())
				assetManager.ReleaseAsset<eng::ShaderAsset>(sprite->m_Shader);
			if (sprite->m_Texture2D.IsValid())
				assetManager.ReleaseAsset<eng::Texture2DAsset>(sprite->m_Texture2D);
		}
		assetManager.ReleaseAsset<eng::SpriteAsset>(component.m_Sprite);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const eng::StaticMeshComponent>>())
	{
		const auto& component = world.ReadComponent<eng::StaticMeshComponent>(entity, false);
		if (!component.m_StaticMesh.IsValid())
			continue;

		assetManager.ReleaseAsset<eng::StaticMeshAsset>(component.m_StaticMesh);
	}

}