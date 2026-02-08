#include "TacticsPCH.h"
#include "Tactics/TacticsAbilityComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAbilities = "m_Abilities";
}

template<>
void eng::Visitor::ReadCustom(tactics::AbilityComponent& value) const
{
	Read(strAbilities, value.m_Abilities, value.m_Abilities);
}
template<>
void eng::Visitor::WriteCustom(const tactics::AbilityComponent& value)
{
	Write(strAbilities, value.m_Abilities);
}
template<>
bool imgui::Inspector::WriteCustom(tactics::AbilityComponent& value)
{
	bool result = false;
	result |= Write("m_Abilities", value.m_Abilities);
	return result;
}