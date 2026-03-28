#include "TacticsPCH.h"
#include "Tactics/TacticsPawnAbilitiesComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAbilities = "m_Abilities";
}

template<>
void Visitor::ReadCustom(tactics::PawnAbilitiesComponent& value) const
{
	Read(strAbilities, value.m_Abilities, value.m_Abilities);
}
template<>
void Visitor::WriteCustom(const tactics::PawnAbilitiesComponent& value)
{
	Write(strAbilities, value.m_Abilities);
}
template<>
bool imgui::Inspector::WriteCustom(tactics::PawnAbilitiesComponent& value)
{
	bool result = false;
	result |= Write("m_Abilities", value.m_Abilities);
	return result;
}