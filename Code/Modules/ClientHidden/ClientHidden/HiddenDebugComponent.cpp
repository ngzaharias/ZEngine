#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenDebugComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strIsInputEnabled = "m_IsInputEnabled";
	const str::StringView strIsObjectEnabled = "m_IsObjectEnabled";
}

template<>
void Visitor::ReadCustom(client::hidden::DebugComponent& value) const
{
	Read(strIsInputEnabled, value.m_IsInputEnabled, value.m_IsInputEnabled);
	Read(strIsObjectEnabled, value.m_IsObjectEnabled, value.m_IsObjectEnabled);
}
template<>
void Visitor::WriteCustom(const client::hidden::DebugComponent& value)
{
	Write(strIsInputEnabled, value.m_IsInputEnabled);
	Write(strIsObjectEnabled, value.m_IsObjectEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_IsInputEnabled", value.m_IsInputEnabled);
	result |= Write("m_IsObjectEnabled", value.m_IsObjectEnabled);
	return result;
}