#include "EnginePCH.h"
#include "Engine/SettingsLaunchStaticComponent.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strLevel = "m_Level";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LaunchStaticComponent& value) const
{
	Read(strLevel, value.m_Level, value.m_Level);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LaunchStaticComponent& value)
{
	Write(strLevel, value.m_Level);
}
