#pragma once

#include <NsApp/DelegateCommand.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/INotifyPropertyChanged.h>

namespace gui
{
	class DCGameMenu final : public Noesis::BaseComponent
	{
	public:
		DCGameMenu();

		const NoesisApp::DelegateCommand* GetExitGameCommand() const { return &m_ExitGameCommand; }
		const NoesisApp::DelegateCommand* GetExitToMenuCommand() const { return &m_ExitToMenuCommand; }
		const NoesisApp::DelegateCommand* GetResumeCommand() const { return &m_ResumeCommand; }
		const NoesisApp::DelegateCommand* GetSettingsCommand() const { return &m_SettingsCommand; }

	private:
		void OnExitGameCommand(Noesis::BaseComponent* param);
		void OnExitToMenuCommand(Noesis::BaseComponent* param);
		void OnResumeCommand(Noesis::BaseComponent* param);
		void OnSettingsCommand(Noesis::BaseComponent* param);

	private:
		NoesisApp::DelegateCommand m_ExitGameCommand = {};
		NoesisApp::DelegateCommand m_ExitToMenuCommand = {};
		NoesisApp::DelegateCommand m_ResumeCommand = {};
		NoesisApp::DelegateCommand m_SettingsCommand = {};

		NS_DECLARE_REFLECTION(gui::DCGameMenu, Noesis::BaseComponent)
	};
}