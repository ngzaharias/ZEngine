#include "EnginePCH.h"
#include "Engine/GameplaySettingsComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strTheme = "m_Theme";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::GameplayComponent& value) const
{
	Read(strTheme, value.m_Theme, value.m_Theme);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::GameplayComponent& value)
{
	Write(strTheme, value.m_Theme);
}