#include "TacticsPCH.h"
#include "Tactics/TacticsSkillTable.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strPath = "m_Path";
}

template<>
void eng::Visitor::ReadCustom(tactics::Move& value) const
{
	Read(strPath, value.m_Path, value.m_Path);
}
template<>
void eng::Visitor::ReadCustom(tactics::Teleport& value) const
{
	Read(strPath, value.m_Path, value.m_Path);
}