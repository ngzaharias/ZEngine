#include "GameUIPCH.h"
#include "GameUI/InputBindingsComponent.h"

#include "Serialize/Visitor.h"

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