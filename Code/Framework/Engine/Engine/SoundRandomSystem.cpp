#include "EnginePCH.h"
#include "Engine/SoundRandomSystem.h"

#include "Core/Random.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundRandomBufferSingleton.h"
#include "Engine/SoundRandomComponent.h"
#include "Engine/SoundRandomRequestComponent.h"

namespace
{
	static int32 s_IndexPrev = -1;
}

void eng::sound::RandomSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& bufferComponent = world.WriteSingleton<eng::sound::RandomBufferSingleton>();
	bufferComponent.m_Requests.RemoveAll();

	using Query = ecs::query
		::Added<const eng::sound::RandomRequestComponent>
		::Include<const eng::sound::RandomRequestComponent>
		::Optional<const eng::sound::RandomComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const auto& requestComponent = view.ReadRequired<eng::sound::RandomRequestComponent>();
		if (!requestComponent.m_Asset.IsValid())
			continue;

		// #temp: request and fetch in the same frame
		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.RequestAsset(requestComponent.m_Asset);
		if (const auto* randomAsset = assetManager.ReadAsset<eng::sound::RandomAsset>(requestComponent.m_Asset))
		{
			const int32 count = randomAsset->m_Handles.GetCount();

			auto& sequenceComponent = !view.HasOptional<eng::sound::RandomComponent>()
				? world.AddComponent<eng::sound::RandomComponent>(view)
				: *view.WriteOptional<eng::sound::RandomComponent>();
			sequenceComponent.m_Index = random::Range(0, count - 1);
			sequenceComponent.m_Asset = requestComponent.m_Asset;

			if (sequenceComponent.m_Index == s_IndexPrev)
				sequenceComponent.m_Index = (sequenceComponent.m_Index + 1) % count;
			s_IndexPrev = sequenceComponent.m_Index;

			str::Guid& singleHandle = bufferComponent.m_Requests.Emplace();
			singleHandle = randomAsset->m_Handles[sequenceComponent.m_Index];
		}
		assetManager.ReleaseAsset(requestComponent.m_Asset);
	}
}

