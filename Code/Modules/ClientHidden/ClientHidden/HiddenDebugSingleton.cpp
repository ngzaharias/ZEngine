#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenDebugSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsInputEnabled = "m_IsInputEnabled";
	const str::StringView strIsObjectEnabled = "m_IsObjectEnabled";
}

template<>
void eng::Visitor::ReadCustom(client::hidden::DebugSingleton& value) const
{
	Read(strIsInputEnabled, value.m_IsInputEnabled, value.m_IsInputEnabled);
	Read(strIsObjectEnabled, value.m_IsObjectEnabled, value.m_IsObjectEnabled);
}
template<>
void eng::Visitor::WriteCustom(const client::hidden::DebugSingleton& value)
{
	Write(strIsInputEnabled, value.m_IsInputEnabled);
	Write(strIsObjectEnabled, value.m_IsObjectEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::DebugSingleton& value)
{
	bool result = false;
	result |= Write("m_IsInputEnabled", value.m_IsInputEnabled);
	result |= Write("m_IsObjectEnabled", value.m_IsObjectEnabled);
	return result;
}