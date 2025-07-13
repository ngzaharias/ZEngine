#include "GameUIPCH.h"

#include "GameUI/DCMainMenu.h"

#include <NsCore/ReflectionImplement.h>

gui::DCMainMenu::DCMainMenu()
{
	m_NewGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnNewGameCommand));
	m_ContinueGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnContinueGameCommand));
	m_LoadGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnLoadGameCommand));
	m_SettingsCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnSettingsCommand));
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnExitGameCommand));
}

void gui::DCMainMenu::OnNewGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "New Game");
}

void gui::DCMainMenu::OnContinueGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Continue Game");
}

void gui::DCMainMenu::OnLoadGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Load Game");
}

void gui::DCMainMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Settings");
}

void gui::DCMainMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Exit Game");
}

NS_IMPLEMENT_REFLECTION(gui::DCMainMenu)
{
	NsProp("NewGameCommand", &gui::DCMainMenu::GetNewGameCommand);
	NsProp("ContinueGameCommand", &gui::DCMainMenu::GetContinueGameCommand);
	NsProp("LoadGameCommand", &gui::DCMainMenu::GetLoadGameCommand);
	NsProp("SettingsCommand", &gui::DCMainMenu::GetSettingsCommand);
	NsProp("ExitGameCommand", &gui::DCMainMenu::GetExitGameCommand);
}