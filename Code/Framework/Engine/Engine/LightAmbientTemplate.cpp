#include "EnginePCH.h"
#include "Engine/LightAmbientTemplate.h"

#include "Serialize/Visitor.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
}

template<>
void Visitor::ReadCustom(eng::light::AmbientTemplate& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void Visitor::WriteCustom(const eng::light::AmbientTemplate& value)
{
	Write(strColour, value.m_Colour);
}