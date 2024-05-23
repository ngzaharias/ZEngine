#include "EnginePCH.h"
#include "Engine/SoundRandomSystem.h"

#include <Core/Random.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Engine/AssetManager.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundComponents.h"

void eng::sound::RandomSystem::Initialise(World& world)
{
	world.AddSingleton<eng::sound::RandomBufferComponent>();
}

void eng::sound::RandomSystem::Shutdown(World& world)
{
	world.RemoveSingleton<eng::sound::RandomBufferComponent>();
}

void eng::sound::RandomSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.GetResource<eng::AssetManager>();

	auto& bufferComponent = world.GetSingleton<eng::sound::RandomBufferComponent>();
	bufferComponent.m_Requests.RemoveAll();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::sound::RandomRequestComponent>>())
	{
		const auto& requestComponent = world.GetComponent<const eng::sound::RandomRequestComponent>(entity);
		if (!requestComponent.m_Handle.IsValid())
			continue;

		const auto* randomAsset = assetManager.LoadAsset<eng::sound::RandomAsset>(requestComponent.m_Handle);
		if (!randomAsset)
			continue;

		const int32 count = randomAsset->m_Handles.GetCount();

		auto& sequenceComponent = world.HasComponent<eng::sound::RandomComponent>(entity)
			? world.GetComponent<eng::sound::RandomComponent>(entity)
			: world.AddComponent<eng::sound::RandomComponent>(entity);
		sequenceComponent.m_Index = random::Range(0, count - 1);
		sequenceComponent.m_Handle = requestComponent.m_Handle;

		str::Guid& singleHandle = bufferComponent.m_Requests.Emplace();
		singleHandle = randomAsset->m_Handles[sequenceComponent.m_Index];
	}
}

