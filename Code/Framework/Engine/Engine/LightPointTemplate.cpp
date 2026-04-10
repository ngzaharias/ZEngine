#include "EnginePCH.h"
#include "Engine/LightPointTemplate.h"

#include "Serialize/Visitor.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strRange = "m_Range";
}

template<>
void Visitor::ReadCustom(eng::light::PointTemplate& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
	Read(strRange, value.m_Range, value.m_Range);
}
template<>
void Visitor::WriteCustom(const eng::light::PointTemplate& value)
{
	Write(strColour, value.m_Colour);
	Write(strRange, value.m_Range);
}