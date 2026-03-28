#include "GameClientPCH.h"
#include "GameClient/SettingsSystem.h"

#include "Camera/CameraSettingsComponent.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponent.h"
#include "Engine/SettingsAudioComponent.h"
#include "Engine/SettingsGameplayComponent.h"
#include "Engine/SettingsWindowComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strAudio = "Audio";
	const str::StringView strCamera = "Camera";
	const str::StringView strGameplay = "Gameplay";
	const str::StringView strWindow = "Window";
	const str::StringView strFilename = "GameSettings.toml";
}

void client::SettingsSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	auto& audioSettings = world.WriteComponent<eng::settings::AudioComponent>();
	auto& cameraSettings = world.WriteComponent<camera::SettingsComponent>();
	auto& gameplaySettings = world.WriteComponent<eng::settings::GameplayComponent>();
	auto& windowSettings = world.WriteComponent<eng::settings::WindowComponent>();
	if (const eng::Window* window = windowManager.GetWindow(0))
	{
		windowSettings.m_Resolution = window->GetSize();
		windowSettings.m_RefreshRate = window->GetRefreshRate();
	}

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(strAudio, audioSettings, {});
	visitor.Read(strCamera, cameraSettings, {});
	visitor.Read(strGameplay, gameplaySettings, {});
	visitor.Read(strWindow, windowSettings, {});
}

void client::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const bool hasChanged =
		world.HasAny<ecs::query::Updated<eng::settings::AudioComponent>>() ||
		world.HasAny<ecs::query::Updated<camera::SettingsComponent>>() ||
		world.HasAny<ecs::query::Updated<eng::settings::GameplayComponent>>() ||
		world.HasAny<ecs::query::Updated<eng::settings::WindowComponent>>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		Visitor visitor;
		visitor.Write(strAudio, world.ReadComponent<eng::settings::AudioComponent>());
		visitor.Write(strCamera, world.ReadComponent<camera::SettingsComponent>());
		visitor.Write(strGameplay, world.ReadComponent<eng::settings::GameplayComponent>());
		visitor.Write(strWindow, world.ReadComponent<eng::settings::WindowComponent>());
		visitor.SaveToFile(filepath);
	}
}

