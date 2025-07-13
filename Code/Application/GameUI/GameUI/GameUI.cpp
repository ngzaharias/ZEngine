#include "GameUIPCH.h"
#include "GameUI/GameUI.h"

#include "ECS/EntityWorld.h"
#include "Engine/PrototypeManager.h"
#include "Engine/UIManager.h"
#include "GameUI/DCMainMenu.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/GameMenuSystem.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/InputBindingsSystem.h"
#include "GameUI/InputComponents.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/MainMenuSystem.h"
#include "GameUI/SettingsComponents.h"
#include "GameUI/SettingsMenuSystem.h"

namespace
{
	const str::Name strMainMenu = NAME("MainMenu.xaml");
}

gui::GameUI::GameUI(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void gui::GameUI::Register(const Dependencies& dependencies)
{
	// components
	{
		m_EntityWorld.RegisterComponent<gui::game_menu::OpenRequestComponent>();
		m_EntityWorld.RegisterComponent<gui::game_menu::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::input::BindingsComponent>();
		m_EntityWorld.RegisterComponent<gui::main_menu::WindowComponent>();
		m_EntityWorld.RegisterComponent<gui::settings::CloseRequestComponent>();
		m_EntityWorld.RegisterComponent<gui::settings::OpenRequestComponent>();
		m_EntityWorld.RegisterComponent<gui::settings::WindowComponent>();
	}

	// systems
	{
		m_EntityWorld.RegisterSystem<gui::game_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::hidden::CountSystem>();
		m_EntityWorld.RegisterSystem<gui::hidden::LevelSystem>();
		m_EntityWorld.RegisterSystem<gui::input::BindingsSystem>();
		m_EntityWorld.RegisterSystem<gui::main_menu::MenuSystem>();
		m_EntityWorld.RegisterSystem<gui::settings::MenuSystem>();
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
		uiManager.RegisterDataContext<gui::DCMainMenu>(strMainMenu);
	}
}

void gui::GameUI::Initialise()
{
	// ui
	{
		auto& uiManager = m_EntityWorld.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strMainMenu);
	}
}

void gui::GameUI::Shutdown()
{
}

void gui::GameUI::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}