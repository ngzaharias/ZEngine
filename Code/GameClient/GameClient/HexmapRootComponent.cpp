#include "GameClientPCH.h"
#include "GameClient/HexmapRootComponent.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strDepthMax = "m_DepthMax";
	const str::StringView strDepthMin = "m_DepthMin";
	const str::StringView strHexCount = "m_HexCount";
	const str::StringView strHexRadius = "m_HexRadius";
	const str::StringView strHexRatio = "m_HexRatio";
}

template<>
void eng::Visitor::ReadCustom(hexmap::RootComponent& value) const
{
	Read(strDepthMin, value.m_DepthMin, value.m_DepthMin);
	Read(strDepthMax, value.m_DepthMax, value.m_DepthMax);
	Read(strHexRatio, value.m_HexRatio, value.m_HexRatio);
	Read(strHexRadius, value.m_HexRadius, value.m_HexRadius);
	Read(strHexCount, value.m_HexCount, value.m_HexCount);
}
template<>
void eng::Visitor::WriteCustom(const hexmap::RootComponent& value)
{
	Write(strDepthMin, value.m_DepthMin);
	Write(strDepthMax, value.m_DepthMax);
	Write(strHexRatio, value.m_HexRatio);
	Write(strHexRadius, value.m_HexRadius);
	Write(strHexCount, value.m_HexCount);
}
template<>
bool imgui::Inspector::WriteCustom(hexmap::RootComponent& value)
{
	bool result = false;
	result |= Write("m_DepthMin", value.m_DepthMin);
	result |= Write("m_DepthMax", value.m_DepthMax);
	result |= Write("m_HexRatio", value.m_HexRatio);
	result |= Write("m_HexRadius", value.m_HexRadius);
	result |= Write("m_HexCount", value.m_HexCount);
	return result;
}