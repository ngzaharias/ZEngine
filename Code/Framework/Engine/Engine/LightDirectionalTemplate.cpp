#include "EnginePCH.h"
#include "Engine/LightDirectionalTemplate.h"

#include "Serialize/Visitor.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
}

template<>
void Visitor::ReadCustom(eng::light::DirectionalTemplate& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void Visitor::WriteCustom(const eng::light::DirectionalTemplate& value)
{
	Write(strColour, value.m_Colour);
}