#include "GameUIPCH.h"
#include "GameUI/GameUI.h"

#include "ECS/EntityWorld.h"
#include "GameUI/HiddenCountSystem.h"
#include "GameUI/HiddenLevelSystem.h"
#include "GameUI/SettingsComponents.h"
#include "GameUI/SettingsMenu.h"

gui::GameUI::GameUI(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void gui::GameUI::Register(const Dependencies& dependencies)
{
	m_EntityWorld.RegisterComponent<gui::settings::CloseRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::OpenRequestComponent>();
	m_EntityWorld.RegisterComponent<gui::settings::WindowComponent>();

	m_EntityWorld.RegisterSystem<gui::hidden::CountSystem>();
	m_EntityWorld.RegisterSystem<gui::hidden::LevelSystem>();
	m_EntityWorld.RegisterSystem<gui::settings::MenuSystem>();
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