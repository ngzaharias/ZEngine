#include "GameUIPCH.h"
#include "GameUI/GameUI.h"

#include "ECS/EntityWorld.h"
#include "Engine/PrototypeManager.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/MainMenu.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/SettingsComponents.h"
#include "GameUI/SettingsMenu.h"

gui::GameUI::GameUI(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void gui::GameUI::Register(const Dependencies& dependencies)
{
	m_EntityWorld.RegisterComponent<gui::main_menu::WindowComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::CloseRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::OpenRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::WindowComponent>();

	m_EntityWorld.RegisterSystem<gui::main_menu::MainMenu>();
	m_EntityWorld.RegisterSystem<gui::hidden::CountSystem>();
	m_EntityWorld.RegisterSystem<gui::hidden::LevelSystem>();
	m_EntityWorld.RegisterSystem<gui::settings::MenuSystem>();

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