#include "GameUIPCH.h"
#include "GameUI/InputComponents.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strBindings = "m_Bindings";
	const str::StringView strCommand = "m_Command";
	const str::StringView strInput = "m_Input";
}

template<>
void eng::Visitor::ReadCustom(gui::input::Binding& value) const
{
	Read(strCommand, value.m_Command, value.m_Command);
	Read(strInput, value.m_Input, value.m_Input);
}
template<>
void eng::Visitor::WriteCustom(const gui::input::Binding& value)
{
	Write(strCommand, value.m_Command);
	Write(strInput, value.m_Input);
}

template<>
void eng::Visitor::ReadCustom(gui::input::BindingsComponent& value) const
{
	Read(strBindings, value.m_Bindings, value.m_Bindings);
}
template<>
void eng::Visitor::WriteCustom(const gui::input::BindingsComponent& value)
{
	Write(strBindings, value.m_Bindings);
}