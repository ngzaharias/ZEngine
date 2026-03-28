#include "GameUIPCH.h"
#include "GameUI/InputComponents.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strCommands = "m_Commands";
}

template<>
void Visitor::ReadCustom(gui::input::BindingsComponent& value) const
{
	Read(strCommands, value.m_Commands, value.m_Commands);
}
template<>
void Visitor::WriteCustom(const gui::input::BindingsComponent& value)
{
	Write(strCommands, value.m_Commands);
}
template<>
bool imgui::Inspector::WriteCustom(gui::input::BindingsComponent& value)
{
	bool result = false;
	result |= Write("m_Commands", value.m_Commands);
	return result;
}