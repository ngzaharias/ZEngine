#include "EnginePCH.h"
#include "Engine/AchievementTable.h"

#include "Serialize/Visitor.h"

namespace 
{
	const str::StringView strIdentifier = "m_Identifier";
}

template<>
void Visitor::ReadCustom(eng::Achievement& value) const
{
	Read(strIdentifier, value.m_Identifier, value.m_Identifier);
}