#include "EnginePCH.h"
#include "Engine/SettingsLaunchSingleton.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strLevel = "m_Level";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::LaunchSingleton& value) const
{
	Read(strLevel, value.m_Level, value.m_Level);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::LaunchSingleton& value)
{
	Write(strLevel, value.m_Level);
}
