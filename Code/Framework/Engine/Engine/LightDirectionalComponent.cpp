#include "EnginePCH.h"
#include "Engine/LightDirectionalComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace 
{
	const str::StringView strColour = "m_Colour";
}

template<>
void Visitor::ReadCustom(eng::light::DirectionalComponent& value) const
{
	Read(strColour, value.m_Colour, value.m_Colour);
}
template<>
void Visitor::WriteCustom(const eng::light::DirectionalComponent& value)
{
	Write(strColour, value.m_Colour);
}
template<>
bool imgui::Inspector::WriteCustom(eng::light::DirectionalComponent& value)
{
	bool result = false;
	result |= Write("m_Colour", value.m_Colour);
	return result;
}