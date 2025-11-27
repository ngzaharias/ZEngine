#include "HiddenPCH.h"
#include "Hidden/HiddenDebugSettingsComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsInputEnabled = "m_IsInputEnabled";
	const str::StringView strIsObjectEnabled = "m_IsObjectEnabled";
}

template<>
void eng::Visitor::ReadCustom(hidden::settings::DebugSingleton& value) const
{
	Read(strIsInputEnabled, value.m_IsInputEnabled, value.m_IsInputEnabled);
	Read(strIsObjectEnabled, value.m_IsObjectEnabled, value.m_IsObjectEnabled);
}
template<>
void eng::Visitor::WriteCustom(const hidden::settings::DebugSingleton& value)
{
	Write(strIsInputEnabled, value.m_IsInputEnabled);
	Write(strIsObjectEnabled, value.m_IsObjectEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::settings::DebugSingleton& value)
{
	bool result = false;
	result |= Write("m_IsInputEnabled", value.m_IsInputEnabled);
	result |= Write("m_IsObjectEnabled", value.m_IsObjectEnabled);
	return result;
}