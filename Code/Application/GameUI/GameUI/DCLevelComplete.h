#pragma once

#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/INotifyPropertyChanged.h>

namespace gui
{
	class DCLevelComplete final : public eng::UIDataContext
	{
	public:
		DCLevelComplete();

		const NoesisApp::DelegateCommand* GetExitGameCommand() const { return &m_ExitGameCommand; }
		const NoesisApp::DelegateCommand* GetExitToMenuCommand() const { return &m_ExitToMenuCommand; }

	private:
		void OnExitGameCommand(Noesis::BaseComponent* param);
		void OnExitToMenuCommand(Noesis::BaseComponent* param);

	private:
		NoesisApp::DelegateCommand m_ExitGameCommand = {};
		NoesisApp::DelegateCommand m_ExitToMenuCommand = {};

		NS_DECLARE_REFLECTION(gui::DCLevelComplete, eng::UIDataContext)
	};
}