#include "EnginePCH.h"
#include "Engine/SettingsLaunchComponent.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strLevel = "m_Level";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LaunchComponent& value) const
{
	Read(strLevel, value.m_Level, value.m_Level);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LaunchComponent& value)
{
	Write(strLevel, value.m_Level);
}
