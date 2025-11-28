#include "GameClientPCH.h"
#include "GameClient/SettingsSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/AudioSettingsComponent.h"
#include "Engine/CameraSettingsComponent.h"
#include "Engine/GameplaySettingsComponent.h"
#include "Engine/SettingsWindowSingleton.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Engine/Visitor.h"

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
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	auto& audioSettings = world.WriteSingleton<eng::settings::AudioSingleton>();
	auto& cameraSettings = world.WriteSingleton<eng::settings::CameraSingleton>();
	auto& gameplaySettings = world.WriteSingleton<eng::settings::GameplaySingleton>();
	auto& windowSettings = world.WriteSingleton<eng::settings::WindowSingleton>();
	if (const eng::Window* window = windowManager.GetWindow(0))
	{
		windowSettings.m_Resolution = window->GetSize();
		windowSettings.m_RefreshRate = window->GetRefreshRate();
	}

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(strAudio, audioSettings, {});
	visitor.Read(strCamera, cameraSettings, {});
	visitor.Read(strGameplay, gameplaySettings, {});
	visitor.Read(strWindow, windowSettings, {});
}

void client::SettingsSystem::Update(World& world, const GameTime& gameTime)
{
	const bool hasChanged =
		world.HasAny<eng::settings::AudioSingleton>() || 
		world.HasAny<eng::settings::CameraSingleton>() ||
		world.HasAny<eng::settings::GameplaySingleton>() ||
		world.HasAny<eng::settings::WindowSingleton>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(strAudio, world.ReadSingleton<eng::settings::AudioSingleton>());
		visitor.Write(strCamera, world.ReadSingleton<eng::settings::CameraSingleton>());
		visitor.Write(strGameplay, world.ReadSingleton<eng::settings::GameplaySingleton>());
		visitor.Write(strWindow, world.ReadSingleton<eng::settings::WindowSingleton>());
		visitor.SaveToFile(filepath);
	}
}

