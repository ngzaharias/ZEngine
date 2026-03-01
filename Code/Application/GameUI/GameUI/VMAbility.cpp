#include "GameUIPCH.h"
#include "GameUI/VMAbility.h"

#include "Tactics/TacticsAbilityTable.h"

#include <NsCore/ReflectionImplement.h>

gui::VMAbility::VMAbility(const str::Name& name, const tactics::Ability& ability)
	: m_Name(name)
	, m_Display(ability.m_Display)
	, m_Tooltip(ability.m_Tooltip)
{
}

bool gui::VMAbility::GetIsSelected() const
{
	return m_IsSelected;
}

void gui::VMAbility::SetIsSelected(const bool value)
{
	m_IsSelected = value;
	OnPropertyChanged("IsSelected");
}

NS_IMPLEMENT_REFLECTION(gui::VMAbility)
{
	NsProp("Display", &gui::VMAbility::GetDisplay);
	NsProp("Tooltip", &gui::VMAbility::GetTooltip);
	NsProp("IsSelected", &gui::VMAbility::GetIsSelected);
}