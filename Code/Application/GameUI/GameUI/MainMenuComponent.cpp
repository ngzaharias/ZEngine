#include "GameUIPCH.h"
#include "GameUI/MainMenuComponent.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strNewGame = "m_NewGame";
}

template<>
void eng::Visitor::ReadCustom(gui::MainMenuComponent& value) const
{
	Read(strNewGame, value.m_NewGame, value.m_NewGame);
}

template<>
void eng::Visitor::WriteCustom(const gui::MainMenuComponent& value)
{
	Write(strNewGame, value.m_NewGame);
}