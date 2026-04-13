#include "GameUIPCH.h"
#include "GameUI/MainMenuWindowTemplate.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strNewGame = "m_NewGame";
}

template<>
void Visitor::ReadCustom(gui::main_menu::WindowTemplate& value) const
{
	Read(strNewGame, value.m_NewGame, value.m_NewGame);
}
template<>
void Visitor::WriteCustom(const gui::main_menu::WindowTemplate& value)
{
	Write(strNewGame, value.m_NewGame);
}