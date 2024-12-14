#include "GameClientPCH.h"
#include "GameClient/SettingsComponents.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsHexmapEnabled = "m_IsHexmapEnabled";
}

template<>
void eng::Visitor::ReadCustom(clt::settings::DebugComponent& value) const
{
	Read(strIsHexmapEnabled, value.m_IsHexmapEnabled, value.m_IsHexmapEnabled);
}
template<>
void eng::Visitor::WriteCustom(const clt::settings::DebugComponent& value)
{
	Write(strIsHexmapEnabled, value.m_IsHexmapEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(clt::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_IsHexmapEnabled", value.m_IsHexmapEnabled);
	return result;
}