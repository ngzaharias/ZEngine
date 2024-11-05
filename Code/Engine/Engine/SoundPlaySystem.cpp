#include "EnginePCH.h"
#include "Engine/SoundPlaySystem.h"

#include "Core/Algorithms.h"
#include "Core/Guid.h"
#include "Core/Random.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/SettingsComponents.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundComponents.h"

#include <SFML/Audio/SoundBuffer.hpp>

void eng::sound::PlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
	const auto& audioSettings = localSettings.m_Audio;

	Array<str::Guid> requests;
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::sound::SingleRequestComponent>>())
	{
		const auto& requestComponent = world.ReadComponent<eng::sound::SingleRequestComponent>(entity);
		requests.Append(requestComponent.m_Handle);
	}

	// Random Buffer
	{
		const auto& bufferComponent = world.ReadSingleton<eng::sound::RandomBufferComponent>();
		requests.Append(bufferComponent.m_Requests);
	}

	// Sequence Buffer
	{
		const auto& bufferComponent = world.ReadSingleton<eng::sound::SequenceBufferComponent>();
		requests.Append(bufferComponent.m_Requests);
	}

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	for (const str::Guid& request : requests)
	{
		if (!request.IsValid())
			continue;

		const auto* soundAsset = assetManager.FetchAsset<eng::sound::SingleAsset>(request);
		if (!soundAsset)
			continue;

		const ecs::Entity entity = world.CreateEntity();

		auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
		nameComponent.m_Name = soundAsset->m_Name;

		auto& soundComponent = world.AddComponent<eng::sound::ObjectComponent>(entity);
		soundComponent.m_Sound = new sf::Sound();
		soundComponent.m_Sound->setBuffer(soundAsset->m_SoundBuffer);
		soundComponent.m_Sound->setVolume(static_cast<float>(audioSettings.m_EffectVolume));
		soundComponent.m_Sound->play();
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::sound::ObjectComponent>>())
	{
		const auto& component = world.ReadComponent<eng::sound::ObjectComponent>(entity);
		if (component.m_Sound->getStatus() == sf::Sound::Stopped)
			world.DestroyEntity(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::sound::ObjectComponent>>())
	{
		auto& component = world.WriteComponent<eng::sound::ObjectComponent>(entity, false);
		delete component.m_Sound;
	}
}

