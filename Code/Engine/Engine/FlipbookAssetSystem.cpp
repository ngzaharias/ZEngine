#include "EnginePCH.h"
#include "Engine/FlipbookAssetSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/ShaderAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/Texture2DAsset.h"

namespace
{
	void LoadAsset(eng::AssetManager& assetManager, eng::FlipbookAssetComponent& component, const str::Guid& guid)
	{
		if (!guid.IsValid())
			return;

		assetManager.RequestAsset<eng::FlipbookAsset>(guid);
		if (const auto* flipbook = assetManager.FetchAsset<eng::FlipbookAsset>(guid))
		{
			component.m_Flipbook = flipbook;
			if (flipbook->m_Shader.IsValid())
			{
				assetManager.RequestAsset<eng::ShaderAsset>(flipbook->m_Shader);
				component.m_Shader = assetManager.FetchAsset<eng::ShaderAsset>(flipbook->m_Shader);
			}

			if (flipbook->m_Texture2D.IsValid())
			{
				assetManager.RequestAsset<eng::Texture2DAsset>(flipbook->m_Texture2D);
				component.m_Texture2D = assetManager.FetchAsset<eng::Texture2DAsset>(flipbook->m_Texture2D);
			}
		}
	}

	void UnloadAsset(eng::AssetManager& assetManager, eng::FlipbookAssetComponent& component)
	{
		if (component.m_Shader)
		{
			assetManager.ReleaseAsset<eng::ShaderAsset>(component.m_Shader->m_Guid);
			component.m_Shader = nullptr;
		}

		if (component.m_Flipbook)
		{
			assetManager.ReleaseAsset<eng::FlipbookAsset>(component.m_Flipbook->m_Guid);
			component.m_Flipbook = nullptr;
		}

		if (component.m_Texture2D)
		{
			assetManager.ReleaseAsset<eng::Texture2DAsset>(component.m_Texture2D->m_Guid);
			component.m_Texture2D = nullptr;
		}
	}
}

void eng::FlipbookAssetSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	using AddedQuery = ecs::query
		::Include<eng::FlipbookComponent>
		::Exclude<eng::FlipbookAssetComponent>;
	for (const ecs::Entity& entity : world.Query<AddedQuery>())
	{
		const auto& flipbookComponent = world.ReadComponent<eng::FlipbookComponent>(entity);
		auto& assetComponent = world.AddComponent<eng::FlipbookAssetComponent>(entity);
		LoadAsset(assetManager, assetComponent, flipbookComponent.m_Flipbook);
	}

	using UpdatedQuery = ecs::query
		::Updated<eng::FlipbookComponent>
		::Include<eng::FlipbookAssetComponent>;
	for (const ecs::Entity& entity : world.Query<UpdatedQuery>())
	{
		const auto& flipbookComponent = world.ReadComponent<eng::FlipbookComponent>(entity);
		auto& assetComponent = world.WriteComponent<eng::FlipbookAssetComponent>(entity);

		const str::Guid& source = assetComponent.m_Flipbook ? assetComponent.m_Flipbook->m_Guid : str::Guid::Unassigned;
		const str::Guid& target = flipbookComponent.m_Flipbook;
		if (source != target)
		{
			UnloadAsset(assetManager, assetComponent);
			LoadAsset(assetManager, assetComponent, flipbookComponent.m_Flipbook);
		}
	}

	using RemovedQuery = ecs::query
		::Exclude<eng::FlipbookComponent>
		::Include<eng::FlipbookAssetComponent>;
	for (const ecs::Entity& entity : world.Query<RemovedQuery>())
	{
		auto& assetComponent = world.WriteComponent<eng::FlipbookAssetComponent>(entity);
		UnloadAsset(assetManager, assetComponent);

		world.RemoveComponent<eng::FlipbookAssetComponent>(entity);
	}
}