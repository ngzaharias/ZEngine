#include "EnginePCH.h"
#include "Engine/SettingsDebugSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
	const str::StringView strAreSoftbodyEnabled = "m_AreSoftbodyEnabled";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::DebugSingleton& value) const
{
	Read(strAreLinesEnabled, value.m_AreLinesEnabled, value.m_AreLinesEnabled);
	Read(strArePhysicsEnabled, value.m_ArePhysicsEnabled, value.m_ArePhysicsEnabled);
	Read(strAreSoftbodyEnabled, value.m_AreSoftbodyEnabled, value.m_AreSoftbodyEnabled);
	// Read(strIsEditorModeEnabled, value.m_IsEditorModeEnabled, value.m_IsEditorModeEnabled);
}
template<>
void eng::Visitor::WriteCustom(const eng::settings::DebugSingleton& value)
{
	Write(strAreLinesEnabled, value.m_AreLinesEnabled);
	Write(strArePhysicsEnabled, value.m_ArePhysicsEnabled);
	Write(strAreSoftbodyEnabled, value.m_AreSoftbodyEnabled);
	// Write(strIsEditorModeEnabled, value.m_IsEditorModeEnabled);
}
template<>
bool imgui::Inspector::WriteCustom(eng::settings::DebugSingleton& value)
{
	bool result = false;
	result |= Write("m_AreLinesEnabled", value.m_AreLinesEnabled);
	result |= Write("m_ArePhysicsEnabled", value.m_ArePhysicsEnabled);
	result |= Write("m_AreSoftbodyEnabled", value.m_AreSoftbodyEnabled);
	result |= Write("m_IsEditorModeEnabled", value.m_IsEditorModeEnabled);
	return result;
}
