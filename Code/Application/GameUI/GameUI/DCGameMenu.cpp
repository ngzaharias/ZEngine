#include "GameUIPCH.h"

#include "ECS/EntityWorld.h"
#include "GameUI/DCGameMenu.h"
#include "GameUI/GameMenuComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCGameMenu::DCGameMenu()
{
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnExitGameCommand));
	m_ExitToMenuCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnExitToMenuCommand));
	m_ResumeCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnResumeCommand));
	m_SettingsCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnSettingsCommand));
}

void gui::DCGameMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::ExitGameRequest>();
}

void gui::DCGameMenu::OnExitToMenuCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::ExitToMenuRequest>();
}

void gui::DCGameMenu::OnResumeCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::CloseRequest>();
}

void gui::DCGameMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::SettingsRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCGameMenu)
{
	NsProp("ExitGameCommand", &gui::DCGameMenu::GetExitGameCommand);
	NsProp("ExitToMenuCommand", &gui::DCGameMenu::GetExitToMenuCommand);
	NsProp("ResumeCommand", &gui::DCGameMenu::GetResumeCommand);
	NsProp("SettingsCommand", &gui::DCGameMenu::GetSettingsCommand);
}