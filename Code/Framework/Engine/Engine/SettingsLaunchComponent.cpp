#include "EnginePCH.h"
#include "Engine/SettingsLaunchComponent.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strLevel = "m_Level";
}

template<>
void Visitor::ReadCustom(eng::settings::LaunchComponent& value) const
{
	Read(strLevel, value.m_Level, value.m_Level);
}
template<>
void Visitor::WriteCustom(const eng::settings::LaunchComponent& value)
{
	Write(strLevel, value.m_Level);
}
