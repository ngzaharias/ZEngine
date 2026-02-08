#include "GameUIPCH.h"
#include "GameUI/MainMenuWindowComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strNewGame = "m_NewGame";
}

template<>
void eng::Visitor::ReadCustom(gui::main_menu::WindowComponent& value) const
{
	Read(strNewGame, value.m_NewGame, value.m_NewGame);
}
template<>
void eng::Visitor::WriteCustom(const gui::main_menu::WindowComponent& value)
{
	Write(strNewGame, value.m_NewGame);
}
template<>
bool imgui::Inspector::WriteCustom(gui::main_menu::WindowComponent& value)
{
	bool result = false;
	result |= Write("m_NewGame", value.m_NewGame);
	return result;
}