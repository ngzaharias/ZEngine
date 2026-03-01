#pragma once

#include "Core/Name.h"
#include "Engine/UIViewModel.h"

namespace tactics
{
	struct Ability;
}

namespace gui
{
	struct VMAbility final : public eng::UIViewModel
	{
	public:
		VMAbility(const str::Name& name, const tactics::Ability& ability);

		const str::Name& GetName() const { return m_Name; }
		const char* GetDisplay() const { return m_Display.c_str(); }
		const char* GetTooltip() const { return m_Tooltip.c_str(); }

		bool GetIsSelected() const;
		void SetIsSelected(const bool value);

	private:
		str::Name m_Name = {};
		str::String m_Display = {};
		str::String m_Tooltip = {};
		bool m_IsSelected = false;

	private:
		NS_DECLARE_REFLECTION(gui::VMAbility, Noesis::BaseComponent)
	};
}