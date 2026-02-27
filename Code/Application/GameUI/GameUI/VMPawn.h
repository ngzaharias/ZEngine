#pragma once

#include "Core/Name.h"
#include "Engine/UIViewModel.h"
#include "GameUI/VMAbility.h"

namespace Noesis
{
	template<class T>
	class ObservableCollection;
}

namespace gui
{
	class VMPawn final : public eng::UIViewModel
	{
	public:
		VMPawn(const str::String& name);

		const char* GetName() const;

		Noesis::ObservableCollection<gui::VMAbility>* GetAbilities() const;
		void SetAbilities(Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> value);

	public:
		Noesis::String m_Name = {};
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> m_Abilities;

	private:
		NS_DECLARE_REFLECTION(gui::VMPawn, Noesis::BaseComponent)
	};
}