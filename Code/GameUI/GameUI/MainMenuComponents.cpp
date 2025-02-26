#include "GameUIPCH.h"
#include "GameUI/MainMenuComponents.h"

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