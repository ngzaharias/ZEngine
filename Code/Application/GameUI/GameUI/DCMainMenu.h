#pragma once

#include <NsApp/DelegateCommand.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/INotifyPropertyChanged.h>

namespace gui
{
	class DCMainMenu final : public Noesis::BaseComponent
	{
	public:
		DCMainMenu();

		const NoesisApp::DelegateCommand* GetContinueGameCommand() const { return &m_ContinueGameCommand; }
		const NoesisApp::DelegateCommand* GetExitGameCommand() const { return &m_ExitGameCommand; }
		const NoesisApp::DelegateCommand* GetLoadGameCommand() const { return &m_LoadGameCommand; }
		const NoesisApp::DelegateCommand* GetNewGameCommand() const { return &m_NewGameCommand; }
		const NoesisApp::DelegateCommand* GetSettingsCommand() const { return &m_SettingsCommand; }

	private:
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

		NS_DECLARE_REFLECTION(gui::DCMainMenu, Noesis::BaseComponent)
	};
}