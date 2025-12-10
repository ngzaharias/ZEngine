#include "GameClientPCH.h"
#include "GameClient/SettingsDebugComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsHexmapEnabled = "m_IsHexmapEnabled";
}

template<>
void eng::Visitor::ReadCustom(client::settings::DebugSingleton& value) const
{
	Read(strIsHexmapEnabled, value.m_IsHexmapEnabled, value.m_IsHexmapEnabled);
}
template<>
void eng::Visitor::WriteCustom(const client::settings::DebugSingleton& value)
{
	Write(strIsHexmapEnabled, value.m_IsHexmapEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(client::settings::DebugSingleton& value)
{
	bool result = false;
	result |= Write("m_IsHexmapEnabled", value.m_IsHexmapEnabled);
	return result;
}