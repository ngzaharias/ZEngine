#include "EnginePCH.h"
#include "Engine/MusicSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/AudioHelpers.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/AudioSettingsComponent.h"

namespace
{
	const str::Guid strAsset = GUID("0640370595b247cfa579e0dd794a6324");
}

void eng::MusicSystem::Initialise(World& world)
{
	auto& musicComponent = world.WriteSingleton<eng::MusicComponent>();
	musicComponent.m_Music = new sf::Music();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	// #todo: trigger music from somewhere else
	assetManager.RequestAsset(strAsset);
	if (const auto* musicAsset = assetManager.ReadAsset<eng::MusicAsset>(strAsset))
	{
		const str::Path filepath = str::Path(str::EPath::Assets, musicAsset->m_SourceFile);

		musicComponent.m_Music->setVolume(0.f);
		musicComponent.m_Music->openFromFile(filepath.ToChar());
		musicComponent.m_Music->play();
	}
}

void eng::MusicSystem::Shutdown(World& world)
{
	auto& component = world.WriteSingleton<eng::MusicComponent>();
	delete component.m_Music;

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strAsset);
}

void eng::MusicSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// update the music volume when it changes
	if (world.HasAny<ecs::query::Updated<const eng::settings::AudioComponent>>())
	{
		const auto& audioSettings = world.ReadSingleton<eng::settings::AudioComponent>();
		auto& musicComponent = world.WriteSingleton<eng::MusicComponent>();
		const float volume = audio::ToVolume(audioSettings.m_MusicVolume * audioSettings.m_MasterVolume);
		musicComponent.m_Music->setVolume(volume);
		musicComponent.m_Music->setLoop(true);
	}
}

