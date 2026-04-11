#include "EnginePCH.h"
#include "Engine/SettingsDebugComponent.h"

#include "Serialize/Visitor.h"

#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
	const str::StringView strAreSoftbodyEnabled = "m_AreSoftbodyEnabled";
}

template<>
void Visitor::ReadCustom(eng::settings::DebugComponent& value) const
{
	Read(strAreLinesEnabled, value.m_AreLinesEnabled, value.m_AreLinesEnabled);
	Read(strArePhysicsEnabled, value.m_ArePhysicsEnabled, value.m_ArePhysicsEnabled);
	Read(strAreSoftbodyEnabled, value.m_AreSoftbodyEnabled, value.m_AreSoftbodyEnabled);
	// Read(strIsEditorModeEnabled, value.m_IsEditorModeEnabled, value.m_IsEditorModeEnabled);
}
template<>
void Visitor::WriteCustom(const eng::settings::DebugComponent& value)
{
	Write(strAreLinesEnabled, value.m_AreLinesEnabled);
	Write(strArePhysicsEnabled, value.m_ArePhysicsEnabled);
	Write(strAreSoftbodyEnabled, value.m_AreSoftbodyEnabled);
	// Write(strIsEditorModeEnabled, value.m_IsEditorModeEnabled);
}
template<>
bool imgui::WriteCustom(eng::settings::DebugComponent& value)
{
	bool result = false;
	result |= Write("m_AreLinesEnabled", value.m_AreLinesEnabled);
	result |= Write("m_ArePhysicsEnabled", value.m_ArePhysicsEnabled);
	result |= Write("m_AreSoftbodyEnabled", value.m_AreSoftbodyEnabled);
	return result;
}
