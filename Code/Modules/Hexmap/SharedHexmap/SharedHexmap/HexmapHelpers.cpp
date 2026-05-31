#include "HexmapPCH.h"
#include "SharedHexmap/HexmapData.h"

#include "SharedHexmap/HexmapHelpers.h"
#include "Math/Vector.h"

shared::hexmap::LayerPos shared::hexmap::ToLayerPos(const HexPos& hexPos, const Vector2i& stride)
{
	return LayerPos(hexPos.x / stride.x, hexPos.y / stride.y);
}

shared::hexmap::HexPos shared::hexmap::ToHexPos(const LayerPos& hexPos, const Vector2i& stride)
{
	return HexPos(hexPos.x * stride.x, hexPos.y * stride.y);
}