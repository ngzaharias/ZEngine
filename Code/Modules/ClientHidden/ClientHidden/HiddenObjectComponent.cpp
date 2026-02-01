#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenObjectComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strGroup = "m_Group";
}

template<>
void eng::Visitor::ReadCustom(client::hidden::ObjectComponent& value) const
{
	Read(strGroup, value.m_Group, value.m_Group);
}
template<>
void eng::Visitor::WriteCustom(const client::hidden::ObjectComponent& value)
{
	Write(strGroup, value.m_Group);
}
template<>
bool imgui::Inspector::WriteCustom(client::hidden::ObjectComponent& value)
{
	bool result = false;
	result |= Write("m_Group", value.m_Group);
	return result;
}