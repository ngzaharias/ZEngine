#include "GameUIPCH.h"
#include "GameUI/InputBindingsTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strCommands = "m_Commands";
}

template<>
void Visitor::ReadCustom(gui::input::BindingsTemplate& value) const
{
	Read(strCommands, value.m_Commands, value.m_Commands);
}
template<>
void Visitor::WriteCustom(const gui::input::BindingsTemplate& value)
{
	Write(strCommands, value.m_Commands);
}