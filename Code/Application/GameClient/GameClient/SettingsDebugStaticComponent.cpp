#include "GameClientPCH.h"
#include "GameClient/SettingsDebugStaticComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsHexmapEnabled = "m_IsHexmapEnabled";
}

template<>
void eng::Visitor::ReadCustom(client::settings::DebugStaticComponent& value) const
{
	Read(strIsHexmapEnabled, value.m_IsHexmapEnabled, value.m_IsHexmapEnabled);
}
template<>
void eng::Visitor::WriteCustom(const client::settings::DebugStaticComponent& value)
{
	Write(strIsHexmapEnabled, value.m_IsHexmapEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(client::settings::DebugStaticComponent& value)
{
	bool result = false;
	result |= Write("m_IsHexmapEnabled", value.m_IsHexmapEnabled);
	return result;
}