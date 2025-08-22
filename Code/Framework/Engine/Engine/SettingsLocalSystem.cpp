#include "EnginePCH.h"
#include "Engine/SettingsLocalSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/AudioSettingsComponent.h"
#include "Engine/CameraSettingsComponent.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Engine/WindowSettingsComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strAudio = "Audio";
	const str::StringView strCamera = "Camera";
	const str::StringView strWindow = "Window";
	const str::StringView strFilename = "GameSettings.toml";
}

void eng::settings::LocalSystem::Initialise(World& world)
{
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	auto& audioSettings = world.WriteSingleton<eng::settings::AudioComponent>();
	auto& cameraSettings = world.WriteSingleton<eng::settings::CameraComponent>();
	auto& windowSettings = world.WriteSingleton<eng::settings::WindowComponent>();
	if (const eng::Window* window = windowManager.GetWindow(0))
	{
		windowSettings.m_Resolution = window->GetResolution();
		windowSettings.m_RefreshRate = window->GetRefreshRate();
	}

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(strAudio, audioSettings, {});
	visitor.Read(strCamera, cameraSettings, {});
	visitor.Read(strWindow, windowSettings, {});
}

void eng::settings::LocalSystem::Update(World& world, const GameTime& gameTime)
{
	const bool hasChanged = 
		world.HasAny<ecs::query::Updated<const eng::settings::AudioComponent>>() || 
		world.HasAny<ecs::query::Updated<const eng::settings::CameraComponent>>() || 
		world.HasAny<ecs::query::Updated<const eng::settings::WindowComponent>>();
	if (hasChanged)
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(strAudio, world.ReadSingleton<eng::settings::AudioComponent>());
		visitor.Write(strCamera, world.ReadSingleton<eng::settings::CameraComponent>());
		visitor.Write(strWindow, world.ReadSingleton<eng::settings::WindowComponent>());
		visitor.SaveToFile(filepath);
	}
}

