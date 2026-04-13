#include "HexmapPCH.h"
#include "Hexmap/HexmapRootTemplate.h"

#include "Engine/AssetManager.h"
#include "Engine/InspectorHelpers.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strHexCount = "m_HexCount";
	const str::StringView strHexRadius = "m_HexRadius";
}

template<>
void Visitor::ReadCustom(hexmap::RootTemplate& value) const
{
	Read(strHexRadius, value.m_HexRadius, value.m_HexRadius);
	Read(strHexCount, value.m_HexCount, value.m_HexCount);
}
template<>
void Visitor::WriteCustom(const hexmap::RootTemplate& value)
{
	Write(strHexRadius, value.m_HexRadius);
	Write(strHexCount, value.m_HexCount);
}