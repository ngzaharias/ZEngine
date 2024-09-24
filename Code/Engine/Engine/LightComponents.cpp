#include "EnginePCH.h"
#include "Engine/LightComponents.h"

#include "Engine/Visitor.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strRange = "m_Range";
}

template<>
void eng::Visitor::ReadCustom(eng::LightAmbientComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void eng::Visitor::WriteCustom(const eng::LightAmbientComponent& value)
{
	Write(strColour, value.m_Colour);
}

template<>
void eng::Visitor::ReadCustom(eng::LightDirectionalComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void eng::Visitor::WriteCustom(const eng::LightDirectionalComponent& value)
{
	Write(strColour, value.m_Colour);
}

template<>
void eng::Visitor::ReadCustom(eng::LightPointComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
	Read(strRange, value.m_Range, value.m_Range);
}
template<>
void eng::Visitor::WriteCustom(const eng::LightPointComponent& value)
{
	Write(strColour, value.m_Colour);
	Write(strRange, value.m_Range);
}