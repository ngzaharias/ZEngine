#include "EnginePCH.h"
#include "Engine/SoundPlaySystem.h"

#include <Core/Algorithms.h>
#include <Core/Guid.h>
#include <Core/Random.h>

#include <ECS/EntityWorld.h>
#include <ECS/NameComponent.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <SFML/Audio/SoundBuffer.hpp>

#include "Engine/AssetManager.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundComponents.h"

// #todo: load volume from config

void eng::sound::PlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();


	Array<str::Guid> requests;
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const eng::sound::SingleRequestComponent>>())
	{
		const auto& requestComponent = world.GetComponent<const eng::sound::SingleRequestComponent>(entity);
		requests.Append(requestComponent.m_Handle);
	}

	// Random Buffer
	{
		const auto& bufferComponent = world.GetSingleton<const eng::sound::RandomBufferComponent>();
		requests.Append(bufferComponent.m_Requests);
	}

	// Sequence Buffer
	{
		const auto& bufferComponent = world.GetSingleton<const eng::sound::SequenceBufferComponent>();
		requests.Append(bufferComponent.m_Requests);
	}

	auto& assetManager = world.GetResource<eng::AssetManager>();
	for (const str::Guid& request : requests)
	{
		if (!request.IsValid())
			continue;

		const auto* soundAsset = assetManager.LoadAsset<eng::sound::SingleAsset>(request);
		if (!soundAsset)
			continue;

		const ecs::Entity entity = world.CreateEntity();

		auto& nameComponent = world.AddComponent<ecs::NameComponent>(entity);
		nameComponent.m_Name = soundAsset->m_Name;

		auto& soundComponent = world.AddComponent<eng::sound::ObjectComponent>(entity);
		soundComponent.m_Sound = new sf::Sound();
		soundComponent.m_Sound->setBuffer(soundAsset->m_SoundBuffer);
		soundComponent.m_Sound->setVolume(m_Volume);
		soundComponent.m_Sound->play();
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::sound::ObjectComponent>>())
	{
		auto& component = world.GetComponent<const eng::sound::ObjectComponent>(entity);
		if (component.m_Sound->getStatus() == sf::Sound::Stopped)
			world.DestroyEntity(entity);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::sound::ObjectComponent>>())
	{
		auto& component = world.GetComponent<eng::sound::ObjectComponent>(entity, false);
		delete component.m_Sound;
	}
}

