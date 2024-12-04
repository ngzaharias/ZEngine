#include "GameClientPCH.h"
#include "GameClient/HexmapRootComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strHexCount = "m_HexCount";
	const str::StringView strHexRadius = "m_HexRadius";
}

template<>
void eng::Visitor::ReadCustom(hexmap::RootComponent& value) const
{
	Read(strHexRadius, value.m_HexRadius, value.m_HexRadius);
	Read(strHexCount, value.m_HexCount, value.m_HexCount);
}
template<>
void eng::Visitor::WriteCustom(const hexmap::RootComponent& value)
{
	Write(strHexRadius, value.m_HexRadius);
	Write(strHexCount, value.m_HexCount);
}
template<>
bool imgui::Inspector::WriteCustom(hexmap::RootComponent& value)
{
	bool result = false;
	result |= Write("m_HexRadius", value.m_HexRadius);
	result |= Write("m_HexCount", value.m_HexCount);
	return result;
}