#include "GameClientPCH.h"
#include "GameClient/SettingsComponents.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsHexmapEnabled = "m_IsHexmapEnabled";
	const str::StringView strIsHiddenInputEnabled = "m_IsHiddenInputEnabled";
	const str::StringView strIsHiddenObjectEnabled = "m_IsHiddenObjectEnabled";
}

template<>
void eng::Visitor::ReadCustom(clt::settings::DebugComponent& value) const
{
	Read(strIsHexmapEnabled, value.m_IsHexmapEnabled, value.m_IsHexmapEnabled);
	Read(strIsHiddenInputEnabled, value.m_IsHiddenInputEnabled, value.m_IsHiddenInputEnabled);
	Read(strIsHiddenObjectEnabled, value.m_IsHiddenObjectEnabled, value.m_IsHiddenObjectEnabled);
}
template<>
void eng::Visitor::WriteCustom(const clt::settings::DebugComponent& value)
{
	Write(strIsHexmapEnabled, value.m_IsHexmapEnabled);
	Write(strIsHiddenInputEnabled, value.m_IsHiddenInputEnabled);
	Write(strIsHiddenObjectEnabled, value.m_IsHiddenObjectEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(clt::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_IsHexmapEnabled", value.m_IsHexmapEnabled);
	result |= Write("m_IsHiddenInputEnabled", value.m_IsHiddenInputEnabled);
	result |= Write("m_IsHiddenObjectEnabled", value.m_IsHiddenObjectEnabled);
	return result;
}