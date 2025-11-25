#include "GameUIPCH.h"
#include "GameUI/GameUI.h"

#include "ECS/EntityWorld.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/DCHiddenCount.h"
#include "GameUI/DCLevelComplete.h"
#include "GameUI/DCLoadingScreen.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/GameMenuSystem.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/HintTable.h"
#include "GameUI/InputBindingsSystem.h"
#include "GameUI/InputComponents.h"
#include "GameUI/LevelCompleteComponents.h"
#include "GameUI/LevelCompleteSystem.h"
#include "GameUI/LoadingComponents.h"
#include "GameUI/LoadingSystem.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/MainMenuSystem.h"
#include "GameUI/SettingsMenuComponents.h"
#include "GameUI/SettingsMenuSystem.h"
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

gui::GameUI::GameUI(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void gui::GameUI::Register(const Dependencies& dependencies)
{
	// components
	{
		m_EntityWorld.RegisterComponent<gui::game_menu::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::input::BindingsComponent>();
		m_EntityWorld.RegisterComponent<gui::level_complete::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::main_menu::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::settings_menu::WindowComponent>();
	}

	// events
	{
		m_EntityWorld.RegisterEvent<gui::game_menu::CloseRequest>();
		m_EntityWorld.RegisterEvent<gui::game_menu::ExitGameRequest>();
		m_EntityWorld.RegisterEvent<gui::game_menu::ExitToMenuRequest>();
		m_EntityWorld.RegisterEvent<gui::game_menu::OpenRequest>();
		m_EntityWorld.RegisterEvent<gui::level_complete::ExitGameRequest>();
		m_EntityWorld.RegisterEvent<gui::level_complete::ExitToMenuRequest>();
		m_EntityWorld.RegisterEvent<gui::level_complete::ResetGameRequest>();
		m_EntityWorld.RegisterEvent<gui::loading::CloseRequest>();
		m_EntityWorld.RegisterEvent<gui::main_menu::ContinueGameRequest>();
		m_EntityWorld.RegisterEvent<gui::main_menu::ExitGameRequest>();
		m_EntityWorld.RegisterEvent<gui::main_menu::LoadGameRequest>();
		m_EntityWorld.RegisterEvent<gui::main_menu::NewGameRequest>();
		m_EntityWorld.RegisterEvent<gui::settings_menu::CloseRequest>();
		m_EntityWorld.RegisterEvent<gui::settings_menu::OpenRequest>();
		m_EntityWorld.RegisterEvent<gui::settings_menu::ValueRequest>();
	}

	// systems
	{
		m_EntityWorld.RegisterSystem<gui::game_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::hidden::CountSystem>();
		m_EntityWorld.RegisterSystem<gui::hidden::LevelSystem>();
		m_EntityWorld.RegisterSystem<gui::input::BindingsSystem>();
		m_EntityWorld.RegisterSystem<gui::level_complete::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::loading::LoadingSystem>();
		m_EntityWorld.RegisterSystem<gui::main_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::settings_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::ThemeSystem>();
	}

	// prototypes
	{
		auto& manager = dependencies.m_PrototypeManager;
		manager.Register<gui::input::BindingsComponent>();
		manager.Register<gui::main_menu::WindowComponent>();
	}

	// tables
	{
		auto& headmaster = dependencies.m_Headmaster;
		headmaster.Register<gui::HintTable>("Hints");

		m_EntityWorld.RegisterResource(headmaster.GetManager<gui::HintTable>());
	}

	// ui
	{
		auto& uiManager = dependencies.m_UIManager;
		uiManager.RegisterDataContext<gui::DCGameMenu>(strGameMenu_xaml);
		uiManager.RegisterDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);
		uiManager.RegisterDataContext<gui::DCLevelComplete>(strLevelComplete_xaml);
		uiManager.RegisterDataContext<gui::DCLoadingScreen>(strLoading_xaml);
		uiManager.RegisterDataContext<gui::DCLoadingScreen>(strSplash_xaml);
		uiManager.RegisterDataContext<gui::DCMainMenu>(strMainMenu_xaml);
		uiManager.RegisterDataContext<gui::DCSettingsMenu>(strSettingsMenu_xaml);
	}
}

void gui::GameUI::Initialise()
{
}

void gui::GameUI::Shutdown()
{
}

void gui::GameUI::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}