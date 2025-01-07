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
		requests.Append(requestComponent.m_Asset);
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

	for (const str::Guid& request : requests)
	{
		if (!request.IsValid())
			continue;

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.RequestAsset<eng::sound::SingleAsset>(request);
		if (const auto* soundAsset = assetManager.FetchAsset<eng::sound::SingleAsset>(request))
		{
			const ecs::Entity entity = world.CreateEntity();

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = soundAsset->m_Name;

			auto& object = world.AddComponent<eng::sound::ObjectComponent>(entity);
			object.m_Sound = new sf::Sound();
			object.m_Sound->setBuffer(soundAsset->m_SoundBuffer);
			object.m_Sound->setVolume(static_cast<float>(audioSettings.m_EffectVolume));
			object.m_Sound->play();
			object.m_Asset = request;
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::sound::ObjectComponent>>())
	{
		const auto& object = world.ReadComponent<eng::sound::ObjectComponent>(entity);
		if (object.m_Sound->getStatus() == sf::Sound::Stopped)
			world.DestroyEntity(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::sound::ObjectComponent>>())
	{
		auto& object = world.WriteComponent<eng::sound::ObjectComponent>(entity, false);
		delete object.m_Sound;

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.ReleaseAsset<eng::sound::SingleAsset>(object.m_Asset);
	}
}

