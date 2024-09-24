#include "EnginePCH.h"
#include "Engine/SettingsComponents.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strAreGizmosEnabled = "m_AreGizmosEnabled";
	const str::StringView strAreHiddenEnabled = "m_AreHiddenEnabled";
	const str::StringView strAreLinesEnabled = "m_AreLinesEnabled";
	const str::StringView strArePhysicsEnabled = "m_ArePhysicsEnabled";
}

template<>
void eng::Visitor::ReadCustom(eng::settings::DebugComponent& value) const
{
	Read(strAreGizmosEnabled, value.m_AreGizmosEnabled, value.m_AreGizmosEnabled);
	Read(strAreHiddenEnabled, value.m_AreHiddenEnabled, value.m_AreHiddenEnabled);
	Read(strAreLinesEnabled, value.m_AreLinesEnabled, value.m_AreLinesEnabled);
	Read(strArePhysicsEnabled, value.m_ArePhysicsEnabled, value.m_ArePhysicsEnabled);
}

template<>
void eng::Visitor::WriteCustom(const eng::settings::DebugComponent& value)
{
	Write(strAreGizmosEnabled, value.m_AreGizmosEnabled);
	Write(strAreHiddenEnabled, value.m_AreHiddenEnabled);
	Write(strAreLinesEnabled, value.m_AreLinesEnabled);
	Write(strArePhysicsEnabled, value.m_ArePhysicsEnabled);
}
