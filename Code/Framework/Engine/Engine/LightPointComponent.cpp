#include "EnginePCH.h"
#include "Engine/LightPointComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
	const str::StringView strRange = "m_Range";
}

template<>
void eng::Visitor::ReadCustom(eng::light::PointComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
	Read(strRange, value.m_Range, value.m_Range);
}
template<>
void eng::Visitor::WriteCustom(const eng::light::PointComponent& value)
{
	Write(strColour, value.m_Colour);
	Write(strRange, value.m_Range);
}
template<>
bool imgui::Inspector::WriteCustom(eng::light::PointComponent& value)
{
	bool result = false;
	result |= Write("m_Colour", value.m_Colour);
	result |= Write("m_Range", value.m_Range);
	return result;
}