#include "EnginePCH.h"
#include "Engine/MusicSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/AudioHelpers.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicSingleton.h"
#include "Engine/SettingsAudioSingleton.h"

namespace
{
	const str::Guid strAsset = GUID("0640370595b247cfa579e0dd794a6324");
}

void eng::MusicSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	// #todo: trigger music from somewhere else
	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strAsset);
}

void eng::MusicSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strAsset);
}

void eng::MusicSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// update the music volume when it changes
	if (world.HasAny<eng::settings::AudioSingleton>())
	{
		auto& assetManager = world.WriteResource<eng::AssetManager>();
		if (const auto* musicAsset = assetManager.ReadAsset<eng::MusicAsset>(strAsset))
		{
			const auto& settings = world.ReadSingleton<eng::settings::AudioSingleton>();
			const float volume = audio::ToVolume(settings.m_MusicVolume * settings.m_MasterVolume);
			musicAsset->m_Buffer->setVolume(volume);
			musicAsset->m_Buffer->setLoop(true);
		}
	}
}

