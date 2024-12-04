#include "EnginePCH.h"
#include "Engine/TextAssetSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FontAsset.h"
#include "Engine/TextComponent.h"

namespace
{
	void LoadAsset(eng::AssetManager& assetManager, eng::TextAssetComponent& component, const str::Guid& guid)
	{
		if (!guid.IsValid())
			return;

		assetManager.RequestAsset<eng::FontAsset>(guid);
		component.m_Font = assetManager.FetchAsset<eng::FontAsset>(guid);
	}

	void UnloadAsset(eng::AssetManager& assetManager, eng::TextAssetComponent& component)
	{
		if (component.m_Font)
		{
			assetManager.ReleaseAsset<eng::FontAsset>(component.m_Font->m_Guid);
			component.m_Font = nullptr;
		}
	}
}

void eng::TextAssetSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	using AddedQuery = ecs::query
		::Include<eng::TextComponent>
		::Exclude<eng::TextAssetComponent>;
	for (const ecs::Entity& entity : world.Query<AddedQuery>())
	{
		const auto& textComponent = world.ReadComponent<eng::TextComponent>(entity);
		auto& assetComponent = world.AddComponent<eng::TextAssetComponent>(entity);
		LoadAsset(assetManager, assetComponent, textComponent.m_Font);
	}

	using UpdatedQuery = ecs::query
		::Updated<eng::TextComponent>
		::Include<eng::TextAssetComponent>;
	for (const ecs::Entity& entity : world.Query<UpdatedQuery>())
	{
		const auto& textComponent = world.ReadComponent<eng::TextComponent>(entity);
		auto& assetComponent = world.WriteComponent<eng::TextAssetComponent>(entity);

		const str::Guid& source = assetComponent.m_Font ? assetComponent.m_Font->m_Guid : str::Guid::Unassigned;
		const str::Guid& target = textComponent.m_Font;
		if (source != target)
		{
			UnloadAsset(assetManager, assetComponent);
			LoadAsset(assetManager, assetComponent, textComponent.m_Font);
		}
	}

	using RemovedQuery = ecs::query
		::Exclude<eng::TextComponent>
		::Include<eng::TextAssetComponent>;
	for (const ecs::Entity& entity : world.Query<RemovedQuery>())
	{
		auto& assetComponent = world.WriteComponent<eng::TextAssetComponent>(entity);
		UnloadAsset(assetManager, assetComponent);

		world.RemoveComponent<eng::TextAssetComponent>(entity);
	}
}