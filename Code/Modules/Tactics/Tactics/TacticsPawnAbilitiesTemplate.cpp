#include "TacticsPCH.h"
#include "Tactics/TacticsPawnAbilitiesTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strAbilities = "m_Abilities";
}

template<>
void Visitor::ReadCustom(tactics::PawnAbilitiesTemplate& value) const
{
	Read(strAbilities, value.m_Abilities, value.m_Abilities);
}
template<>
void Visitor::WriteCustom(const tactics::PawnAbilitiesTemplate& value)
{
	Write(strAbilities, value.m_Abilities);
}