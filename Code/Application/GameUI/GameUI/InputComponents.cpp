#include "GameUIPCH.h"
#include "GameUI/InputComponents.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strCommands = "m_Commands";
}

template<>
void eng::Visitor::ReadCustom(gui::input::BindingsComponent& value) const
{
	Read(strCommands, value.m_Commands, value.m_Commands);
}
template<>
void eng::Visitor::WriteCustom(const gui::input::BindingsComponent& value)
{
	Write(strCommands, value.m_Commands);
}