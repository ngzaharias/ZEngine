#include "EnginePCH.h"
#include "Engine/SettingsLocalSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/MusicAsset.h"
#include "Engine/MusicComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFilename = "GameSettings.toml";
}

void eng::settings::LocalSystem::Initialise(World& world)
{
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	auto& settingsComponent = world.WriteSingleton<eng::settings::LocalComponent>();
	if (const eng::Window* window = windowManager.GetWindow(0))
	{
		settingsComponent.m_Window.m_Resolution = window->GetResolution();
		settingsComponent.m_Window.m_RefreshRate = window->GetRefreshRate();
	}

	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(settingsComponent);
}

void eng::settings::LocalSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Updated<const eng::settings::LocalComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<eng::settings::LocalComponent>());
		visitor.SaveToFile(filepath);
	}
}

