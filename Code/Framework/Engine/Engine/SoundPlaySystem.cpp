#include "EnginePCH.h"
#include "Engine/SoundPlaySystem.h"

#include "Core/Algorithms.h"
#include "Core/Random.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/AudioHelpers.h"
#include "Engine/SettingsAudioSingleton.h"
#include "Engine/SoundAssets.h"
#include "Engine/SoundObjectComponent.h"
#include "Engine/SoundRandomBufferSingleton.h"
#include "Engine/SoundSequenceBufferSingleton.h"
#include "Engine/SoundSingleRequestComponent.h"

#include <NsGUI/IntegrationAPI.h>
#include <NsGUI/Uri.h>
#include <SFML/Audio/SoundBuffer.hpp>

void eng::sound::PlaySystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	Noesis::GUI::SetPlayAudioCallback(this, 
		[](void* user, const Noesis::Uri& uri, float volume)
		{
			auto* self = reinterpret_cast<eng::sound::PlaySystem*>(user);
			const Noesis::String string = uri.ToString();
			const str::Path path = string.Str();
			const str::StringView stem = path.GetStem();
			if (str::Guid::IsValidString(stem))
			{
				const str::Guid guid = GUID(stem);
				self->m_Requests.Append(guid);
			}
		});
}

void eng::sound::PlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& audioSettings = world.ReadSingleton<eng::settings::AudioSingleton>();
	using SingleQuery = ecs::query
		::Added<const eng::sound::SingleRequestComponent>
		::Include<const eng::sound::SingleRequestComponent>;
	for (auto&& view : world.Query<SingleQuery>())
	{
		const auto& requestComponent = view.ReadRequired<eng::sound::SingleRequestComponent>();
		m_Requests.Append(requestComponent.m_Asset);
	}

	// Random Buffer
	{
		const auto& bufferComponent = world.ReadSingleton<eng::sound::RandomBufferSingleton>();
		m_Requests.Append(bufferComponent.m_Requests);
	}

	// Sequence Buffer
	{
		const auto& bufferComponent = world.ReadSingleton<eng::sound::SequenceBufferSingleton>();
		m_Requests.Append(bufferComponent.m_Requests);
	}

	for (const str::Guid& request : m_Requests)
	{
		if (!request.IsValid())
			continue;

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.RequestAsset(request);
		if (const auto* soundAsset = assetManager.ReadAsset<eng::sound::SingleAsset>(request))
		{
			const float volume = audio::ToVolume(audioSettings.m_EffectVolume * audioSettings.m_MasterVolume);

			const ecs::Entity entity = world.CreateEntity();

			auto& name = world.AddComponent<ecs::NameComponent>(entity);
			name.m_Name = soundAsset->m_Name;

			auto& object = world.AddComponent<eng::sound::ObjectComponent>(entity);
			object.m_Sound = new sf::Sound();
			object.m_Sound->setBuffer(soundAsset->m_SoundBuffer);
			object.m_Sound->setVolume(volume);
			object.m_Sound->play();
			object.m_Asset = request;
		}
	}
	m_Requests.RemoveAll();

	for (auto&& view : world.Query<ecs::query::Include<const eng::sound::ObjectComponent>>())
	{
		const auto& object = view.ReadRequired<eng::sound::ObjectComponent>();
		if (object.m_Sound->getStatus() == sf::Sound::Stopped)
			world.DestroyEntity(view);
	}

	for (auto&& view : world.Query<ecs::query::Removed<eng::sound::ObjectComponent>>())
	{
		auto& object = world.WriteComponent<eng::sound::ObjectComponent>(view, false);
		delete object.m_Sound;

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		assetManager.ReleaseAsset(object.m_Asset);
	}
}

