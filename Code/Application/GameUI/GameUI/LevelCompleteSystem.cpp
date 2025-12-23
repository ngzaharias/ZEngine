#include "GameUIPCH.h"
#include "GameUI/LevelCompleteSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ApplicationCloseRequest.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadRequest.h"
#include "Engine/SettingsLaunchSingleton.h"
#include "Engine/UIManager.h"
#include "GameUI/DCLevelComplete.h"
#include "GameUI/LevelCompleteExitGameRequest.h"
#include "GameUI/LevelCompleteExitToMenuRequest.h"
#include "GameUI/LevelCompleteResetGameRequest.h"
#include "GameUI/LevelCompleteWindowComponent.h"

namespace
{
	const str::Name strLevelComplete_xaml = NAME("LevelComplete.xaml");

	const str::StringView strFilename = "Savegame.toml";
}

void gui::level_complete::MenuSystem::Update(World& world, const GameTime& gameTime)
{
	for (auto&& view : world.Query<ecs::query::Added<gui::level_complete::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strLevelComplete_xaml, true);
	}

	for (auto&& view : world.Query<ecs::query::Removed<gui::level_complete::WindowComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strLevelComplete_xaml);
	}

	//////////////////////////////////////////////////////////////////////////
	// Events

	if (world.HasAny<gui::level_complete::ExitGameRequest>())
	{
		world.AddEvent<eng::application::CloseRequest>();
	}

	if (world.HasAny<gui::level_complete::ExitToMenuRequest>())
	{
		const auto& settings = world.ReadSingleton<eng::settings::LaunchSingleton>();
		world.AddEvent<eng::level::LoadRequest>(settings.m_Level);

		for (auto&& view : world.Query<ecs::query::Include<const gui::level_complete::WindowComponent>>())
			world.DestroyEntity(view);
	}

	if (world.HasAny<gui::level_complete::ResetGameRequest>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);
		std::remove(filepath.ToChar());

		for (auto&& view : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
		{
			const auto& levelComponent = view.ReadRequired<eng::level::LoadedComponent>();
			world.AddEvent<eng::level::LoadRequest>(levelComponent.m_Name);
		}

		for (auto&& view : world.Query<ecs::query::Include<gui::level_complete::WindowComponent>>())
			world.DestroyEntity(view);
	}
}