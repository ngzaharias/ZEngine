#include "GameUIPCH.h"
#include "GameUI/DCMainMenu.h"

#include "ECS/EntityWorld.h"
#include "Engine/ExecuteShell.h"
#include "GameUI/MainMenuContinueGameEvent.h"
#include "GameUI/MainMenuExitGameEvent.h"
#include "GameUI/MainMenuLoadGameEvent.h"
#include "GameUI/MainMenuNewGameEvent.h"
#include "GameUI/SettingsMenuOpenEvent.h"

#include <NsCore/ReflectionImplement.h>

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
	m_EntityWorld->AddEvent<gui::main_menu::ContinueGameEvent>();
}

void gui::DCMainMenu::OnExitGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEvent<gui::main_menu::ExitGameEvent>();
}

void gui::DCMainMenu::OnLoadGameCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEvent<gui::main_menu::LoadGameEvent>();
}

void gui::DCMainMenu::OnNewGameCommand(Noesis::BaseComponent* param)
{
	auto& request = m_EntityWorld->AddEvent<gui::main_menu::NewGameEvent>();
	request.m_Level = m_NewGameLevel;
}

void gui::DCMainMenu::OnReportCommand(Noesis::BaseComponent* param)
{
	const char* recipient = "support@papa-koala.com";
	const char* subject = "Tactichess";
	const char* body =
		"Issue:%0D%0A"
		"%0D%0A"
		"Attachments:%0D%0A"
		"%0D%0A";

	const str::String mailto = std::format("mailto:{}?subject={}&body={}", recipient, subject, body);
	eng::ExecuteShell("open", mailto.c_str(), eng::EShellMode::NORMAL);
}

void gui::DCMainMenu::OnSettingsCommand(Noesis::BaseComponent* param)
{
	m_EntityWorld->AddEvent<gui::settings_menu::OpenEvent>();
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