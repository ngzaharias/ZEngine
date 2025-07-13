#include "GameUIPCH.h"

#include "GameUI/DCMainMenu.h"

#include <NsCore/ReflectionImplement.h>

gui::DCMainMenu::DCMainMenu()
{
	m_ContinueGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnContinueGameCommand));
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnExitGameCommand));
	m_LoadGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnLoadGameCommand));
	m_NewGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnNewGameCommand));
	m_SettingsCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnSettingsCommand));
}

void gui::DCMainMenu::OnContinueGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Continue Game");
}

void gui::DCMainMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Exit Game");
}

void gui::DCMainMenu::OnLoadGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Load Game");
}

void gui::DCMainMenu::OnNewGameCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "New Game");
}

void gui::DCMainMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	Z_LOG(ELog::Debug, "Settings");
}

NS_IMPLEMENT_REFLECTION(gui::DCMainMenu)
{
	NsProp("ContinueGameCommand", &gui::DCMainMenu::GetContinueGameCommand);
	NsProp("ExitGameCommand", &gui::DCMainMenu::GetExitGameCommand);
	NsProp("LoadGameCommand", &gui::DCMainMenu::GetLoadGameCommand);
	NsProp("NewGameCommand", &gui::DCMainMenu::GetNewGameCommand);
	NsProp("SettingsCommand", &gui::DCMainMenu::GetSettingsCommand);
}