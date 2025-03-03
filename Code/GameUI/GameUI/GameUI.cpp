#include "GameUIPCH.h"
#include "GameUI/GameUI.h"

#include "ECS/EntityWorld.h"
#include "Engine/PrototypeManager.h"
#include "GameUI/GameMenuSystem.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/InputBindingsSystem.h"
#include "GameUI/InputComponents.h"
#include "GameUI/MainMenuSystem.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/SettingsComponents.h"
#include "GameUI/SettingsMenuSystem.h"

gui::GameUI::GameUI(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void gui::GameUI::Register(const Dependencies& dependencies)
{
	m_EntityWorld.RegisterComponent<gui::game_menu::OpenRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::game_menu::WindowComponent>();
	m_EntityWorld.RegisterComponent<gui::input::BindingsComponent>();
	m_EntityWorld.RegisterComponent<gui::main_menu::WindowComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::CloseRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::OpenRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::WindowComponent>();

	m_EntityWorld.RegisterSystem<gui::game_menu::MenuSystem>();
	m_EntityWorld.RegisterSystem<gui::hidden::CountSystem>();
	m_EntityWorld.RegisterSystem<gui::hidden::LevelSystem>();
	m_EntityWorld.RegisterSystem<gui::input::BindingsSystem>();
	m_EntityWorld.RegisterSystem<gui::main_menu::MenuSystem>();
	m_EntityWorld.RegisterSystem<gui::settings::MenuSystem>();

	dependencies.m_PrototypeManager.Register<gui::input::BindingsComponent>();
	dependencies.m_PrototypeManager.Register<gui::main_menu::WindowComponent>();
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