#pragma once

#include "Core/String.h"
#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>

namespace gui
{
	class DCLoadingScreen final : public eng::UIDataContext
	{
	public:
		DCLoadingScreen();
		~DCLoadingScreen() override;

		const char* GetHint() const;
		void SetHint(const char* value);

		const NoesisApp::DelegateCommand* GetCloseCommand() const { return &m_CloseCommand; }

	private:
		void OnCloseCommand(Noesis::BaseComponent* param);

	private:
		str::String m_Hint = {};

		NoesisApp::DelegateCommand m_CloseCommand = {};

		NS_DECLARE_REFLECTION(gui::DCLoadingScreen, eng::UIDataContext)
	};
}