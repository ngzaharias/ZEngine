#include "TacticsPCH.h"
#include "SharedTactics/TacticsPawnAbilitiesTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strAbilities = "m_Abilities";
}

template<>
void Visitor::ReadCustom(shared::tactics::PawnAbilitiesTemplate& value) const
{
	Read(strAbilities, value.m_Abilities, value.m_Abilities);
}
template<>
void Visitor::WriteCustom(const shared::tactics::PawnAbilitiesTemplate& value)
{
	Write(strAbilities, value.m_Abilities);
}