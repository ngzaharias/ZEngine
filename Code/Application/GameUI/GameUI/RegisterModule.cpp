#include "GameUIPCH.h"
#include "GameUI/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/DCHiddenCount.h"
#include "GameUI/DCLevelComplete.h"
#include "GameUI/DCLoadingScreen.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/GameMenuCloseRequest.h"
#include "GameUI/GameMenuExitGameRequest.h"
#include "GameUI/GameMenuExitToMenuRequest.h"
#include "GameUI/GameMenuOpenRequest.h"
#include "GameUI/GameMenuSystem.h"
#include "GameUI/GameMenuWindowComponent.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/HintTable.h"
#include "GameUI/InputBindingsSystem.h"
#include "GameUI/InputComponents.h"
#include "GameUI/LevelCompleteExitGameRequest.h"
#include "GameUI/LevelCompleteExitToMenuRequest.h"
#include "GameUI/LevelCompleteResetGameRequest.h"
#include "GameUI/LevelCompleteSystem.h"
#include "GameUI/LevelCompleteWindowComponent.h"
#include "GameUI/LoadingCloseRequest.h"
#include "GameUI/LoadingSystem.h"
#include "GameUI/MainMenuContinueGameRequest.h"
#include "GameUI/MainMenuExitGameRequest.h"
#include "GameUI/MainMenuLoadGameRequest.h"
#include "GameUI/MainMenuNewGameRequest.h"
#include "GameUI/MainMenuSystem.h"
#include "GameUI/MainMenuWindowComponent.h"
#include "GameUI/SettingsMenuCloseRequest.h"
#include "GameUI/SettingsMenuOpenRequest.h"
#include "GameUI/SettingsMenuSystem.h"
#include "GameUI/SettingsMenuValueRequest.h"
#include "GameUI/SettingsMenuWindowComponent.h"
#include "GameUI/ThemeSystem.h"

namespace
{
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
	const str::Name strHiddenCount_xaml = NAME("HiddenCount.xaml");
	const str::Name strLevelComplete_xaml = NAME("LevelComplete.xaml");
	const str::Name strLoading_xaml = NAME("Loading.xaml");
	const str::Name strMainMenu_xaml = NAME("MainMenu.xaml");
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
	const str::Name strSplash_xaml = NAME("Splash.xaml");
}

void gui::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<gui::game_menu::WindowComponent>();
		entityWorld.RegisterComponent<gui::input::BindingsComponent>();
		entityWorld.RegisterComponent<gui::level_complete::WindowComponent>();
		entityWorld.RegisterComponent<gui::main_menu::WindowComponent>();
		entityWorld.RegisterComponent<gui::settings_menu::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<gui::game_menu::CloseRequest>();
		entityWorld.RegisterEvent<gui::game_menu::ExitGameRequest>();
		entityWorld.RegisterEvent<gui::game_menu::ExitToMenuRequest>();
		entityWorld.RegisterEvent<gui::game_menu::OpenRequest>();
		entityWorld.RegisterEvent<gui::level_complete::ExitGameRequest>();
		entityWorld.RegisterEvent<gui::level_complete::ExitToMenuRequest>();
		entityWorld.RegisterEvent<gui::level_complete::ResetGameRequest>();
		entityWorld.RegisterEvent<gui::loading::CloseRequest>();
		entityWorld.RegisterEvent<gui::main_menu::ContinueGameRequest>();
		entityWorld.RegisterEvent<gui::main_menu::ExitGameRequest>();
		entityWorld.RegisterEvent<gui::main_menu::LoadGameRequest>();
		entityWorld.RegisterEvent<gui::main_menu::NewGameRequest>();
		entityWorld.RegisterEvent<gui::settings_menu::CloseRequest>();
		entityWorld.RegisterEvent<gui::settings_menu::OpenRequest>();
		entityWorld.RegisterEvent<gui::settings_menu::ValueRequest>();
	}

	// systems
	{
		entityWorld.RegisterSystem<gui::game_menu::MenuSystem>();
		entityWorld.RegisterSystem<gui::hidden::CountSystem>();
		entityWorld.RegisterSystem<gui::hidden::LevelSystem>();
		entityWorld.RegisterSystem<gui::input::BindingsSystem>();
		entityWorld.RegisterSystem<gui::level_complete::MenuSystem>();
		entityWorld.RegisterSystem<gui::loading::LoadingSystem>();
		entityWorld.RegisterSystem<gui::main_menu::MenuSystem>();
		entityWorld.RegisterSystem<gui::settings_menu::MenuSystem>();
		entityWorld.RegisterSystem<gui::ThemeSystem>();
	}

	// prototypes
	{
		auto& manager = entityWorld.WriteResource<eng::PrototypeManager>();
		manager.Register<gui::input::BindingsComponent>();
		manager.Register<gui::main_menu::WindowComponent>();
	}

	// tables
	{
		auto& headmaster = entityWorld.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<gui::HintTable>("Hints");

		entityWorld.RegisterResource(headmaster.GetManager<gui::HintTable>());
	}

	// ui
	{
		auto& manager = entityWorld.WriteResource<eng::UIManager>();
		manager.RegisterDataContext<gui::DCGameMenu>(strGameMenu_xaml);
		manager.RegisterDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);
		manager.RegisterDataContext<gui::DCLevelComplete>(strLevelComplete_xaml);
		manager.RegisterDataContext<gui::DCLoadingScreen>(strLoading_xaml);
		manager.RegisterDataContext<gui::DCLoadingScreen>(strSplash_xaml);
		manager.RegisterDataContext<gui::DCMainMenu>(strMainMenu_xaml);
		manager.RegisterDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
	}
}
