#include "GameUIPCH.h"
#include "GameUI/DCGameMenu.h"

#include "ECS/EntityWorld.h"
#include "GameUI/GameMenuComponents.h"
#include "GameUI/SettingsMenuComponents.h"

#include <NsCore/ReflectionImplement.h>

gui::DCGameMenu::DCGameMenu()
{
	m_CloseCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnCloseCommand));
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnExitGameCommand));
	m_ExitToMenuCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnExitToMenuCommand));
	m_SettingsCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCGameMenu::OnSettingsCommand));
}

void gui::DCGameMenu::OnCloseCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::CloseRequest>();
}

void gui::DCGameMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::ExitGameRequest>();
}

void gui::DCGameMenu::OnExitToMenuCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::game_menu::ExitToMenuRequest>();
}

void gui::DCGameMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::settings_menu::OpenRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCGameMenu)
{
	NsProp("ExitGameCommand", &gui::DCGameMenu::GetExitGameCommand);
	NsProp("ExitToMenuCommand", &gui::DCGameMenu::GetExitToMenuCommand);
	NsProp("CloseCommand", &gui::DCGameMenu::GetCloseCommand);
	NsProp("SettingsCommand", &gui::DCGameMenu::GetSettingsCommand);
}