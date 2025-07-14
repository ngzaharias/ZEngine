#pragma once

#include "Core/Name.h"
#include "Engine/UIWidget.h"

#include <NsApp/DelegateCommand.h>
#include <NsApp/NotifyPropertyChangedBase.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/BaseCommand.h>

namespace gui
{
	class DCMainMenu final : public eng::UIWidget
	{
	public:
		DCMainMenu();

		const char* GetNewGameLevel() const;

		void SetNewGameLevel(const str::Name& value);

	private:
		const NoesisApp::DelegateCommand* GetContinueGameCommand() const { return &m_ContinueGameCommand; }
		const NoesisApp::DelegateCommand* GetExitGameCommand() const { return &m_ExitGameCommand; }
		const NoesisApp::DelegateCommand* GetLoadGameCommand() const { return &m_LoadGameCommand; }
		const NoesisApp::DelegateCommand* GetNewGameCommand() const { return &m_NewGameCommand; }
		const NoesisApp::DelegateCommand* GetSettingsCommand() const { return &m_SettingsCommand; }

		void OnContinueGameCommand(Noesis::BaseComponent* param);
		void OnExitGameCommand(Noesis::BaseComponent* param);
		void OnLoadGameCommand(Noesis::BaseComponent* param);
		void OnNewGameCommand(Noesis::BaseComponent* param);
		void OnSettingsCommand(Noesis::BaseComponent* param);

	private:
		NoesisApp::DelegateCommand m_ContinueGameCommand = {};
		NoesisApp::DelegateCommand m_ExitGameCommand = {};
		NoesisApp::DelegateCommand m_LoadGameCommand = {};
		NoesisApp::DelegateCommand m_NewGameCommand = {};
		NoesisApp::DelegateCommand m_SettingsCommand = {};

		str::Name m_NewGameLevel = {};

		NS_DECLARE_REFLECTION(gui::DCMainMenu, eng::UIWidget)
	};
}