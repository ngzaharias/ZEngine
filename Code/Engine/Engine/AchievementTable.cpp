#include "EnginePCH.h"
#include "Engine/AchievementTable.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strIdentifier = "m_Identifier";
}

template<>
void eng::Visitor::ReadCustom(eng::Achievement& value) const
{
	Read(strIdentifier, value.m_Identifier, value.m_Identifier);
}