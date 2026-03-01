#include "TacticsPCH.h"
#include "Tactics/TacticsAbilityTable.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strDirection = "m_Direction";
	const str::StringView strDisplay = "m_Display";
	const str::StringView strNodes = "m_Nodes";
	const str::StringView strOffset = "m_Offset";
	const str::StringView strSkill = "m_Skill";
	const str::StringView strTooltip = "m_Tooltip";
}

template<>
void eng::Visitor::ReadCustom(tactics::Path& value) const
{
	Read(strNodes, value.m_Nodes, value.m_Nodes);
}

template<>
void eng::Visitor::ReadCustom(tactics::Rush& value) const
{
	Read(strDirection, value.m_Direction, value.m_Direction);
}

template<>
void eng::Visitor::ReadCustom(tactics::Teleport& value) const
{
	Read(strOffset, value.m_Offset, value.m_Offset);
}

template<>
void eng::Visitor::ReadCustom(tactics::Ability& value) const
{
	Read(strDisplay, value.m_Display, value.m_Display);
	Read(strTooltip, value.m_Tooltip, value.m_Tooltip);
	Read(strSkill, value.m_Skill, value.m_Skill);
}