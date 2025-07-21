#include "GameUIPCH.h"
#include "GameUI/GameUI.h"

#include "ECS/EntityWorld.h"
#include "Engine/PrototypeManager.h"
#include "Engine/UIManager.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/DCHiddenCount.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/DCSettingsMenu.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/GameMenuSystem.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/InputBindingsSystem.h"
#include "GameUI/InputComponents.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/MainMenuSystem.h"
#include "GameUI/SettingsMenuComponents.h"
#include "GameUI/SettingsMenuSystem.h"

namespace
{
	const str::Name strGameMenu_xaml = NAME("GameMenu.xaml");
	const str::Name strHiddenCount_xaml = NAME("HiddenCount.xaml");
	const str::Name strMainMenu_xaml = NAME("MainMenu.xaml");
	const str::Name strSettingsMenu_xaml = NAME("SettingsMenu.xaml");
}

gui::GameUI::GameUI(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void gui::GameUI::Register(const Dependencies& dependencies)
{
	// components
	{
		m_EntityWorld.RegisterComponent<gui::game_menu::CloseRequest>();
		m_EntityWorld.RegisterComponent<gui::game_menu::ExitGameRequest>();
		m_EntityWorld.RegisterComponent<gui::game_menu::ExitToMenuRequest>();
		m_EntityWorld.RegisterComponent<gui::game_menu::OpenRequest>();
		m_EntityWorld.RegisterComponent<gui::game_menu::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::input::BindingsComponent>();
		m_EntityWorld.RegisterComponent<gui::main_menu::ContinueGameRequest>();
		m_EntityWorld.RegisterComponent<gui::main_menu::ExitGameRequest>();
		m_EntityWorld.RegisterComponent<gui::main_menu::LoadGameRequest>();
		m_EntityWorld.RegisterComponent<gui::main_menu::NewGameRequest>();
		m_EntityWorld.RegisterComponent<gui::main_menu::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::settings_menu::CloseRequest>();
		m_EntityWorld.RegisterComponent<gui::settings_menu::OpenRequest>();
		m_EntityWorld.RegisterComponent<gui::settings_menu::ValueRequest>();
		m_EntityWorld.RegisterComponent<gui::settings_menu::WindowComponent>();
	}

	// systems
	{
		m_EntityWorld.RegisterSystem<gui::game_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::hidden::CountSystem>();
		m_EntityWorld.RegisterSystem<gui::hidden::LevelSystem>();
		m_EntityWorld.RegisterSystem<gui::input::BindingsSystem>();
		m_EntityWorld.RegisterSystem<gui::main_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::settings_menu::MenuSystem>();
	}

	// prototypes
	{
		auto& manager = dependencies.m_PrototypeManager;
		manager.Register<gui::input::BindingsComponent>();
		manager.Register<gui::main_menu::WindowComponent>();
	}

	// ui
	{
		auto& uiManager = dependencies.m_UIManager;
		uiManager.RegisterDataContext<gui::DCGameMenu>(strGameMenu_xaml);
		uiManager.RegisterDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);
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