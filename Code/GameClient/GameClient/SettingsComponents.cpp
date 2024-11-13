#include "GameClientPCH.h"
#include "GameClient/SettingsComponents.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAreHiddenEnabled = "m_AreHiddenEnabled";
}

template<>
void eng::Visitor::ReadCustom(clt::settings::DebugComponent& value) const
{
	Read(strAreHiddenEnabled, value.m_AreHiddenEnabled, value.m_AreHiddenEnabled);
}
template<>
void eng::Visitor::WriteCustom(const clt::settings::DebugComponent& value)
{
	Write(strAreHiddenEnabled, value.m_AreHiddenEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(clt::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_AreHiddenEnabled", value.m_AreHiddenEnabled);
	return result;
}