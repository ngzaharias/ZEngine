#include "EnginePCH.h"
#include "Engine/SoundSequenceSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundComponents.h"

void eng::sound::SequenceSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	auto& bufferComponent = world.WriteSingleton<eng::sound::SequenceBufferComponent>();
	bufferComponent.m_Requests.RemoveAll();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::sound::SequenceRequestComponent>>())
	{
		const auto& requestComponent = world.ReadComponent< eng::sound::SequenceRequestComponent>(entity);
		if (!requestComponent.m_Handle.IsValid())
			continue;

		const auto* sequenceAsset = assetManager.LoadAsset<eng::sound::SequenceAsset>(requestComponent.m_Handle);
		if (!sequenceAsset)
			continue;

		const int32 count = sequenceAsset->m_Handles.GetCount();

		auto& sequenceComponent = world.HasComponent<eng::sound::SequenceComponent>(entity)
			? world.WriteComponent<eng::sound::SequenceComponent>(entity)
			: world.AddComponent<eng::sound::SequenceComponent>(entity);
		if (sequenceComponent.m_Handle != requestComponent.m_Handle)
		{
			sequenceComponent.m_Index = 0;
		}
		else
		{
			sequenceComponent.m_Index = sequenceComponent.m_Index + 1;
			sequenceComponent.m_Index = sequenceComponent.m_Index % count;
		}

		sequenceComponent.m_Handle = requestComponent.m_Handle;

		str::Guid& singleHandle = bufferComponent.m_Requests.Emplace();
		singleHandle = sequenceAsset->m_Handles[sequenceComponent.m_Index];
	}
}

