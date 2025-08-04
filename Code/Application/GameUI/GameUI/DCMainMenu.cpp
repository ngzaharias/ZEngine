#include "GameUIPCH.h"
#include "GameUI/DCMainMenu.h"

#include "ECS/EntityWorld.h"
#include "GameUI/MainMenuComponents.h"
#include "GameUI/SettingsMenuComponents.h"

#include <NsCore/ReflectionImplement.h>

#include <windows.h>

gui::DCMainMenu::DCMainMenu()
{
	m_ContinueGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnContinueGameCommand));
	m_ExitGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnExitGameCommand));
	m_LoadGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnLoadGameCommand));
	m_NewGameCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnNewGameCommand));
	m_ReportCommand.SetExecuteFunc(MakeDelegate(this, &gui::DCMainMenu::OnReportCommand));
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

const char* gui::DCMainMenu::GetVersion() const
{
	return m_Version.c_str();
}

void gui::DCMainMenu::SetVersion(const str::String& value)
{
	if (m_Version != value)
	{
		m_Version = value;
		OnPropertyChanged("Version");
	}
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

void gui::DCMainMenu::OnReportCommand(Noesis::BaseComponent* param)
{
	const wchar_t* recipient = L"support@papa-koala.com";
	const wchar_t* subject = L"With My Little Eye";
	const wchar_t* body =
		L"Issue:%0D%0A"
		L"%0D%0A"
		L"Attachments:%0D%0A"
		L"%0D%0A";

	// Format mailto URI
	wchar_t mailto[1024];
	swprintf(mailto, 1024,
		L"mailto:%s?subject=%s&body=%s",
		recipient,
		subject,
		body
	);

	ShellExecuteW(nullptr, L"open", mailto, nullptr, nullptr, SW_SHOWNORMAL);
}

void gui::DCMainMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEventComponent<gui::settings_menu::OpenRequest>();
}

NS_IMPLEMENT_REFLECTION(gui::DCMainMenu)
{
	NsProp("Version", &gui::DCMainMenu::GetVersion);

	NsProp("ContinueGameCommand", &gui::DCMainMenu::GetContinueGameCommand);
	NsProp("ExitGameCommand", &gui::DCMainMenu::GetExitGameCommand);
	NsProp("LoadGameCommand", &gui::DCMainMenu::GetLoadGameCommand);
	NsProp("NewGameCommand", &gui::DCMainMenu::GetNewGameCommand);
	NsProp("ReportCommand", &gui::DCMainMenu::GetReportCommand);
	NsProp("SettingsCommand", &gui::DCMainMenu::GetSettingsCommand);
}