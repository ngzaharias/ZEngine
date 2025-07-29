#include "GameUIPCH.h"
#include "GameUI/DCMainMenu.h"

#include "ECS/EntityWorld.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/SettingsMenuComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCMainMenu::DCMainMenu()
{
	m_ContinueGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnContinueGameCommand));
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnExitGameCommand));
	m_LoadGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnLoadGameCommand));
	m_NewGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnNewGameCommand));
	m_SettingsCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnSettingsCommand));
}

gui::DCMainMenu::~DCMainMenu()
{
}

const char* gui::DCMainMenu::GetNewGameLevel() const
{
	return m_NewGameLevel.ToChar();
}

void gui::DCMainMenu::SetNewGameLevel(const str::Name& value)
{
	m_NewGameLevel = value;
}

void gui::DCMainMenu::OnContinueGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::main_menu::ContinueGameRequest>();
}

void gui::DCMainMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::main_menu::ExitGameRequest>();
}

void gui::DCMainMenu::OnLoadGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::main_menu::LoadGameRequest>();
}

void gui::DCMainMenu::OnNewGameCommand(Noesis::BaseComponent* param)
{
	auto& request = m_EntityWorld->AddEventComponent<gui::main_menu::NewGameRequest>();
	request.m_Level = m_NewGameLevel;
}

void gui::DCMainMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::settings_menu::OpenRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCMainMenu)
{
	NsProp("ContinueGameCommand", &gui::DCMainMenu::GetContinueGameCommand);
	NsProp("ExitGameCommand", &gui::DCMainMenu::GetExitGameCommand);
	NsProp("LoadGameCommand", &gui::DCMainMenu::GetLoadGameCommand);
	NsProp("NewGameCommand", &gui::DCMainMenu::GetNewGameCommand);
	NsProp("SettingsCommand", &gui::DCMainMenu::GetSettingsCommand);
}