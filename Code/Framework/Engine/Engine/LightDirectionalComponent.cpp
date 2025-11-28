#include "EnginePCH.h"
#include "Engine/LightDirectionalComponent.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
}

template<>
void eng::Visitor::ReadCustom(eng::light::DirectionalComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void eng::Visitor::WriteCustom(const eng::light::DirectionalComponent& value)
{
	Write(strColour, value.m_Colour);
}