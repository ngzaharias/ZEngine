#include "EnginePCH.h"
#include "Engine/VisibilityComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsVisible = "m_IsVisible";
}

template<>
void Visitor::ReadCustom(eng::VisibilityComponent& value) const
{
	Read(strIsVisible, value.m_IsVisible, value.m_IsVisible);
}
template<>
void Visitor::WriteCustom(const eng::VisibilityComponent& value)
{
	Write(strIsVisible, value.m_IsVisible);
}
template<>
bool imgui::Inspector::WriteCustom(eng::VisibilityComponent& value)
{
	bool result = false;
	result |= imgui::Checkbox("##m_IsVisible", value.m_IsVisible);
	return result;
}