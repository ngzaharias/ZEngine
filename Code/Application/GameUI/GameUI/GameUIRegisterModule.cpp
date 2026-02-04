#include "GameUIPCH.h"
#include "GameUI/GameUIRegisterModule.h"

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

void gui::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<gui::game_menu::WindowComponent>();
		world.RegisterComponent<gui::input::BindingsComponent>();
		world.RegisterComponent<gui::level_complete::WindowComponent>();
		world.RegisterComponent<gui::main_menu::WindowComponent>();
		world.RegisterComponent<gui::settings_menu::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<gui::game_menu::CloseRequest>();
		world.RegisterEvent<gui::game_menu::ExitGameRequest>();
		world.RegisterEvent<gui::game_menu::ExitToMenuRequest>();
		world.RegisterEvent<gui::game_menu::OpenRequest>();
		world.RegisterEvent<gui::level_complete::ExitGameRequest>();
		world.RegisterEvent<gui::level_complete::ExitToMenuRequest>();
		world.RegisterEvent<gui::level_complete::ResetGameRequest>();
		world.RegisterEvent<gui::loading::CloseRequest>();
		world.RegisterEvent<gui::main_menu::ContinueGameRequest>();
		world.RegisterEvent<gui::main_menu::ExitGameRequest>();
		world.RegisterEvent<gui::main_menu::LoadGameRequest>();
		world.RegisterEvent<gui::main_menu::NewGameRequest>();
		world.RegisterEvent<gui::settings_menu::CloseRequest>();
		world.RegisterEvent<gui::settings_menu::OpenRequest>();
		world.RegisterEvent<gui::settings_menu::ValueRequest>();
	}

	// systems
	{
		world.RegisterSystem<gui::game_menu::MenuSystem>();
		world.RegisterSystem<gui::hidden::CountSystem>();
		world.RegisterSystem<gui::hidden::LevelSystem>();
		world.RegisterSystem<gui::input::BindingsSystem>();
		world.RegisterSystem<gui::level_complete::MenuSystem>();
		world.RegisterSystem<gui::loading::LoadingSystem>();
		world.RegisterSystem<gui::main_menu::MenuSystem>();
		world.RegisterSystem<gui::settings_menu::MenuSystem>();
		world.RegisterSystem<gui::ThemeSystem>();
	}

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.Register<gui::input::BindingsComponent>();
		manager.Register<gui::main_menu::WindowComponent>();
	}

	// tables
	{
		auto& headmaster = world.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<gui::HintTable>("Hints");

		world.RegisterResource(headmaster.GetManager<gui::HintTable>());
	}

	// ui
	{
		auto& manager = world.WriteResource<eng::UIManager>();
		manager.RegisterDataContext<gui::DCGameMenu>(strGameMenu_xaml);
		manager.RegisterDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);
		manager.RegisterDataContext<gui::DCLevelComplete>(strLevelComplete_xaml);
		manager.RegisterDataContext<gui::DCLoadingScreen>(strLoading_xaml);
		manager.RegisterDataContext<gui::DCLoadingScreen>(strSplash_xaml);
		manager.RegisterDataContext<gui::DCMainMenu>(strMainMenu_xaml);
		manager.RegisterDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
	}
}
