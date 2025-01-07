#include "EnginePCH.h"
#include "Engine/SoundRandomSystem.h"

#include "Core/Random.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundComponents.h"

void eng::sound::RandomSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& bufferComponent = world.WriteSingleton<eng::sound::RandomBufferComponent>();
	bufferComponent.m_Requests.RemoveAll();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::sound::RandomRequestComponent>>())
	{
		const auto& requestComponent = world.ReadComponent<eng::sound::RandomRequestComponent>(entity);
		if (!requestComponent.m_Asset.IsValid())
			continue;

		// #temp: request and fetch in the same frame
		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.RequestAsset<eng::sound::RandomAsset>(requestComponent.m_Asset);
		if (const auto* randomAsset = assetManager.FetchAsset<eng::sound::RandomAsset>(requestComponent.m_Asset))
		{
			const int32 count = randomAsset->m_Handles.GetCount();

			auto& sequenceComponent = world.HasComponent<eng::sound::RandomComponent>(entity)
				? world.WriteComponent<eng::sound::RandomComponent>(entity)
				: world.AddComponent<eng::sound::RandomComponent>(entity);
			sequenceComponent.m_Index = random::Range(0, count - 1);
			sequenceComponent.m_Asset = requestComponent.m_Asset;

			str::Guid& singleHandle = bufferComponent.m_Requests.Emplace();
			singleHandle = randomAsset->m_Handles[sequenceComponent.m_Index];
		}
		assetManager.ReleaseAsset<eng::sound::RandomAsset>(requestComponent.m_Asset);
	}
}

