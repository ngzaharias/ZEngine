#include "HiddenPCH.h"
#include "Hidden/HiddenGroupComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strObjects = "m_Objects";
	const str::StringView strRevealed = "m_Revealed";
}

template<>
void eng::Visitor::ReadCustom(hidden::GroupComponent& value) const
{
	Read(strObjects, value.m_Objects, value.m_Objects);
	Read(strRevealed, value.m_Revealed, value.m_Revealed);
}
template<>
void eng::Visitor::WriteCustom(const hidden::GroupComponent& value)
{
	Write(strObjects, value.m_Objects);
	Write(strRevealed, value.m_Revealed);
}
template<>
bool imgui::Inspector::WriteCustom(hidden::GroupComponent& value)
{
	const auto& manager = GetPayload<eng::AssetManager>();

	bool result = false;
	result |= Write("m_Objects", value.m_Objects);
	Read("m_Revealed", value.m_Revealed);
	return result;
}