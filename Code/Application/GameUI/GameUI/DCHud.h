#pragma once

#include "Engine/UIDataContext.h"
#include "GameUI/VMPawn.h"

#include <NsApp/DelegateCommand.h>
#include <NsApp/NotifyPropertyChangedBase.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/BaseCommand.h>

namespace Noesis
{
	template<class T>
	class ObservableCollection;
}

namespace gui
{
	class DCHud final : public eng::UIDataContext
	{
	public:
		DCHud();
		~DCHud() override;

		gui::VMPawn* GetSelectedPawn() const;
		void SetSelectedPawn(Noesis::Ptr<gui::VMPawn> value);

	private:
		const NoesisApp::DelegateCommand* GetAbilityExecuteCommand() const { return &m_AbilityExecuteCommand; }
		const NoesisApp::DelegateCommand* GetAbilityPreviewCommand() const { return &m_AbilityPreviewCommand; }

		void OnAbilityExecuteCommand(Noesis::BaseComponent* param);
		void OnAbilityPreviewCommand(Noesis::BaseComponent* param);

	private:
		Noesis::Ptr<gui::VMPawn> m_SelectedPawn;

		NoesisApp::DelegateCommand m_AbilityExecuteCommand = {};
		NoesisApp::DelegateCommand m_AbilityPreviewCommand = {};

		NS_DECLARE_REFLECTION(gui::DCHud, eng::UIDataContext)
	};
}