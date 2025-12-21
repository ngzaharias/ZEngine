#include "EnginePCH.h"
#include "Engine/SoundSequenceSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundSequenceBufferSingleton.h"
#include "Engine/SoundSequenceComponent.h"
#include "Engine/SoundSequenceRequestComponent.h"

void eng::sound::SequenceSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& bufferComponent = world.WriteSingleton<eng::sound::SequenceBufferSingleton>();
	bufferComponent.m_Requests.RemoveAll();

	using Query = ecs::query
		::Added<const eng::sound::SequenceRequestComponent>
		::Include<const eng::sound::SequenceRequestComponent>
		::Optional<const eng::sound::SequenceComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const auto& requestComponent = view.ReadRequired<eng::sound::SequenceRequestComponent>();
		if (!requestComponent.m_Asset.IsValid())
			continue;

		// #temp: request and fetch in the same frame
		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.RequestAsset(requestComponent.m_Asset);
		if (const auto* sequenceAsset = assetManager.ReadAsset<eng::sound::SequenceAsset>(requestComponent.m_Asset))
		{
			const int32 count = sequenceAsset->m_Handles.GetCount();

			auto& sequenceComponent = !view.HasOptional<eng::sound::SequenceComponent>()
				? world.AddComponent<eng::sound::SequenceComponent>(view)
				: *view.WriteOptional<eng::sound::SequenceComponent>();
			if (sequenceComponent.m_Asset != requestComponent.m_Asset)
			{
				sequenceComponent.m_Index = 0;
			}
			else
			{
				sequenceComponent.m_Index = sequenceComponent.m_Index + 1;
				sequenceComponent.m_Index = sequenceComponent.m_Index % count;
			}

			sequenceComponent.m_Asset = requestComponent.m_Asset;

			str::Guid& singleHandle = bufferComponent.m_Requests.Emplace();
			singleHandle = sequenceAsset->m_Handles[sequenceComponent.m_Index];
		}
		assetManager.ReleaseAsset(requestComponent.m_Asset);
	}
}

