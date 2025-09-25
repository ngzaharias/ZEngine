#include "EnginePCH.h"
#include "Engine/ColourTable.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strName = "m_Name";

	const eng::ColourObject s_Fallback = { NAME("_Fallback"), Colour::Magenta };
}

template<>
void eng::Visitor::ReadCustom(eng::ColourObject& value) const
{
	Read(strName, value.m_Name, value.m_Name);
	Read(strColour, value.m_Colour, value.m_Colour);
}

const eng::ColourObject& eng::ColourTable::GetColour(const str::Guid& value) const
{
	const auto find = m_ObjectMap.Find(value);
	if (find != m_ObjectMap.end())
		return find->second;
	return s_Fallback;
}
