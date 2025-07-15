#pragma once

#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/INotifyPropertyChanged.h>

namespace gui
{
	class DCGameMenu final : public eng::UIDataContext
	{
	public:
		DCGameMenu();

		const NoesisApp::DelegateCommand* GetCloseCommand() const { return &m_CloseCommand; }
		const NoesisApp::DelegateCommand* GetExitGameCommand() const { return &m_ExitGameCommand; }
		const NoesisApp::DelegateCommand* GetExitToMenuCommand() const { return &m_ExitToMenuCommand; }
		const NoesisApp::DelegateCommand* GetSettingsCommand() const { return &m_SettingsCommand; }

	private:
		void OnCloseCommand(Noesis::BaseComponent* param);
		void OnExitGameCommand(Noesis::BaseComponent* param);
		void OnExitToMenuCommand(Noesis::BaseComponent* param);
		void OnSettingsCommand(Noesis::BaseComponent* param);

	private:
		NoesisApp::DelegateCommand m_CloseCommand = {};
		NoesisApp::DelegateCommand m_ExitGameCommand = {};
		NoesisApp::DelegateCommand m_ExitToMenuCommand = {};
		NoesisApp::DelegateCommand m_SettingsCommand = {};

		NS_DECLARE_REFLECTION(gui::DCGameMenu, eng::UIDataContext)
	};
}