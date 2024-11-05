#include "EnginePCH.h"
#include "Engine/MusicSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/SettingsComponents.h"

namespace
{
	const str::Guid strAsset = GUID("d193864ee4f444e7b6aabb84b95bcc5b");
}

void eng::MusicSystem::Initialise(World& world)
{
	auto& musicComponent = world.WriteSingleton<eng::MusicComponent>();
	musicComponent.m_Music = new sf::Music();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	if (const auto* musicAsset = assetManager.FetchAsset<eng::MusicAsset>(strAsset))
	{
		const str::Path filepath = str::Path(str::EPath::Assets, musicAsset->m_SourceFile);

		const auto& settingsComponent = world.ReadSingleton<eng::settings::LocalComponent>();
		musicComponent.m_Music->setVolume(0.f);
		musicComponent.m_Music->openFromFile(filepath.ToChar());
		musicComponent.m_Music->play();
	}
}

void eng::MusicSystem::Shutdown(World& world)
{
	auto& component = world.WriteSingleton<eng::MusicComponent>();
	delete component.m_Music;
}

void eng::MusicSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// update the music volume when it changes
	if (world.HasAny<ecs::query::Updated<const eng::settings::LocalComponent>>())
	{
		const auto& localSettings = world.ReadSingleton<eng::settings::LocalComponent>();
		const auto& audioSettings = localSettings.m_Audio;

		auto& musicComponent = world.WriteSingleton<eng::MusicComponent>();
		musicComponent.m_Music->setVolume(static_cast<float>(audioSettings.m_MusicVolume));
	}
}

