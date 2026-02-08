#include "EnginePCH.h"
#include "Engine/LightAmbientComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
}

template<>
void eng::Visitor::ReadCustom(eng::light::AmbientComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void eng::Visitor::WriteCustom(const eng::light::AmbientComponent& value)
{
	Write(strColour, value.m_Colour);
}
template<>
bool imgui::Inspector::WriteCustom(eng::light::AmbientComponent& value)
{
	bool result = false;
	result |= Write("m_Colour", value.m_Colour);
	return result;
}