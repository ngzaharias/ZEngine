#include "GameUIPCH.h"
#include "GameUI/DCMainMenu.h"

#include "ECS/EntityWorld.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/LevelComponents.h"
#include "GameUI/SettingsComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCMainMenu::DCMainMenu()
{
	m_ContinueGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnContinueGameCommand));
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnExitGameCommand));
	m_LoadGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnLoadGameCommand));
	m_NewGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnNewGameCommand));
	m_SettingsCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnSettingsCommand));
}

void gui::DCMainMenu::SetNewGameLevel(const str::Name& value)
{
	m_NewGameLevel = value;
}

const char* gui::DCMainMenu::GetNewGameLevel() const
{
	return m_NewGameLevel.ToChar();
}

void gui::DCMainMenu::OnContinueGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Continue Game");
}

void gui::DCMainMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<eng::application::CloseRequestComponent>();
}

void gui::DCMainMenu::OnLoadGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Load Game");
}

void gui::DCMainMenu::OnNewGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<eng::level::LoadRequestComponent>(m_NewGameLevel);
}

void gui::DCMainMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::settings::OpenRequestComponent>();
}

NS_IMPLEMENT_REFLECTION(gui::DCMainMenu)
{
	NsProp("NewGameLevel", &gui::DCMainMenu::GetNewGameLevel);

	NsProp("ContinueGameCommand", &gui::DCMainMenu::GetContinueGameCommand);
	NsProp("ExitGameCommand", &gui::DCMainMenu::GetExitGameCommand);
	NsProp("LoadGameCommand", &gui::DCMainMenu::GetLoadGameCommand);
	NsProp("NewGameCommand", &gui::DCMainMenu::GetNewGameCommand);
	NsProp("SettingsCommand", &gui::DCMainMenu::GetSettingsCommand);
}