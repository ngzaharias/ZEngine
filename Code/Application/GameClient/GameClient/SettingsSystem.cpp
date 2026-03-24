#include "GameClientPCH.h"
#include "GameClient/SettingsSystem.h"

#include "Camera/CameraSettingsStaticComponent.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicStaticComponent.h"
#include "Engine/SettingsAudioStaticComponent.h"
#include "Engine/SettingsGameplayStaticComponent.h"
#include "Engine/SettingsWindowStaticComponent.h"
#include "Engine/Visitor.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

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
	auto& audioSettings = world.WriteComponent<eng::settings::AudioStaticComponent>();
	auto& cameraSettings = world.WriteComponent<camera::SettingsStaticComponent>();
	auto& gameplaySettings = world.WriteComponent<eng::settings::GameplayStaticComponent>();
	auto& windowSettings = world.WriteComponent<eng::settings::WindowStaticComponent>();
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
	PROFILE_FUNCTION();

	const bool hasChanged =
		world.HasAny<ecs::query::Updated<eng::settings::AudioStaticComponent>>() ||
		world.HasAny<ecs::query::Updated<camera::SettingsStaticComponent>>() ||
		world.HasAny<ecs::query::Updated<eng::settings::GameplayStaticComponent>>() ||
		world.HasAny<ecs::query::Updated<eng::settings::WindowStaticComponent>>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(strAudio, world.ReadComponent<eng::settings::AudioStaticComponent>());
		visitor.Write(strCamera, world.ReadComponent<camera::SettingsStaticComponent>());
		visitor.Write(strGameplay, world.ReadComponent<eng::settings::GameplayStaticComponent>());
		visitor.Write(strWindow, world.ReadComponent<eng::settings::WindowStaticComponent>());
		visitor.SaveToFile(filepath);
	}
}

