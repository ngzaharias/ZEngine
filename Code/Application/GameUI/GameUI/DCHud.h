#pragma once

#include "ECS/WorldView.h"
#include "Engine/UIDataContext.h"
#include "GameUI/VMPawn.h"

#include <NsApp/DelegateCommand.h>
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
		gui::VMPawn* GetSelectedPawn() const;
		void SetSelectedPawn(Noesis::Ptr<gui::VMPawn> value);

	private:
		Noesis::Ptr<gui::VMPawn> m_SelectedPawn;

		NS_DECLARE_REFLECTION(gui::DCHud, eng::UIDataContext)
	};
}