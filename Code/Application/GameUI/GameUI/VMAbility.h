#pragma once

#include "Core/Name.h"
#include "Engine/UIViewModel.h"

namespace gui
{
	class VMAbility final : public eng::UIViewModel
	{
	public:
		VMAbility(const str::Name& name);

		const char* GetName() const;

		bool GetIsSelected() const;
		void SetIsSelected(const bool value);

	public:
		Noesis::String m_Name = {};
		str::Name m_Key = {};
		bool m_IsSelected = false;

	private:
		NS_DECLARE_REFLECTION(gui::VMAbility, Noesis::BaseComponent)
	};
}