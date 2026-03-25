#include "GameUIPCH.h"
#include "GameUI/GameUIRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/DCHiddenCount.h"
#include "GameUI/DCHud.h"
#include "GameUI/DCLevelComplete.h"
#include "GameUI/DCLoadingScreen.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/GameMenuCloseEvent.h"
#include "GameUI/GameMenuExitGameEvent.h"
#include "GameUI/GameMenuExitToMenuEvent.h"
#include "GameUI/GameMenuOpenEvent.h"
#include "GameUI/GameMenuSystem.h"
#include "GameUI/GameMenuWindowComponent.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/HintTable.h"
#include "GameUI/HUDComponent.h"
#include "GameUI/HUDSystem.h"
#include "GameUI/InputBindingsSystem.h"
#include "GameUI/InputComponents.h"
#include "GameUI/LevelCompleteExitGameEvent.h"
#include "GameUI/LevelCompleteExitToMenuEvent.h"
#include "GameUI/LevelCompleteResetGameEvent.h"
#include "GameUI/LevelCompleteSystem.h"
#include "GameUI/LevelCompleteWindowComponent.h"
#include "GameUI/LoadingCloseEvent.h"
#include "GameUI/LoadingSystem.h"
#include "GameUI/MainMenuContinueGameEvent.h"
#include "GameUI/MainMenuExitGameEvent.h"
#include "GameUI/MainMenuLoadGameEvent.h"
#include "GameUI/MainMenuNewGameEvent.h"
#include "GameUI/MainMenuSystem.h"
#include "GameUI/MainMenuWindowComponent.h"
#include "GameUI/SettingsMenuCloseEvent.h"
#include "GameUI/SettingsMenuOpenEvent.h"
#include "GameUI/SettingsMenuSystem.h"
#include "GameUI/SettingsMenuValueEvent.h"
#include "GameUI/SettingsMenuWindowComponent.h"
#include "GameUI/ThemeSystem.h"

namespace
{
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
	const str::Name strHiddenCount_xaml = NAME("HiddenCount.xaml");
	const str::Name strHUD_xaml = NAME("HUD.xaml");
	const str::Name strLevelComplete_xaml = NAME("LevelComplete.xaml");
	const str::Name strLoading_xaml = NAME("Loading.xaml");
	const str::Name strMainMenu_xaml = NAME("MainMenu.xaml");
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
	const str::Name strSplash_xaml = NAME("Splash.xaml");
}

void gui::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<gui::game_menu::WindowComponent>();
	world.RegisterComponent<gui::HUDComponent>();
	world.RegisterComponent<gui::input::BindingsComponent>();
	world.RegisterComponent<gui::level_complete::WindowComponent>();
	world.RegisterComponent<gui::main_menu::WindowComponent>();
	world.RegisterComponent<gui::settings_menu::WindowComponent>();
	world.RegisterEvent<gui::game_menu::CloseEvent>();
	world.RegisterEvent<gui::game_menu::ExitGameEvent>();
	world.RegisterEvent<gui::game_menu::ExitToMenuEvent>();
	world.RegisterEvent<gui::game_menu::OpenEvent>();
	world.RegisterEvent<gui::level_complete::ExitGameEvent>();
	world.RegisterEvent<gui::level_complete::ExitToMenuEvent>();
	world.RegisterEvent<gui::level_complete::ResetGameEvent>();
	world.RegisterEvent<gui::loading::CloseEvent>();
	world.RegisterEvent<gui::main_menu::ContinueGameEvent>();
	world.RegisterEvent<gui::main_menu::ExitGameEvent>();
	world.RegisterEvent<gui::main_menu::LoadGameEvent>();
	world.RegisterEvent<gui::main_menu::NewGameEvent>();
	world.RegisterEvent<gui::settings_menu::CloseEvent>();
	world.RegisterEvent<gui::settings_menu::OpenEvent>();
	world.RegisterEvent<gui::settings_menu::ValueEvent>();
	world.RegisterSystem<gui::game_menu::MenuSystem>();
	world.RegisterSystem<gui::hidden::CountSystem>();
	world.RegisterSystem<gui::hidden::LevelSystem>();
	world.RegisterSystem<gui::HUDSystem>();
	world.RegisterSystem<gui::input::BindingsSystem>();
	world.RegisterSystem<gui::level_complete::MenuSystem>();
	world.RegisterSystem<gui::loading::LoadingSystem>();
	world.RegisterSystem<gui::main_menu::MenuSystem>();
	world.RegisterSystem<gui::settings_menu::MenuSystem>();
	world.RegisterSystem<gui::ThemeSystem>();

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.RegisterComponent<gui::HUDComponent>();
		manager.RegisterComponent<gui::input::BindingsComponent>();
		manager.RegisterComponent<gui::main_menu::WindowComponent>();
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
		manager.RegisterDataContext<gui::DCHud>(strHUD_xaml);
		manager.RegisterDataContext<gui::DCLevelComplete>(strLevelComplete_xaml);
		manager.RegisterDataContext<gui::DCLoadingScreen>(strLoading_xaml);
		manager.RegisterDataContext<gui::DCLoadingScreen>(strSplash_xaml);
		manager.RegisterDataContext<gui::DCMainMenu>(strMainMenu_xaml);
		manager.RegisterDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
	}
}
