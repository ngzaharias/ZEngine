#include "HexmapPCH.h"
#include "Hexmap/HexmapData.h"

#include "Hexmap/HexmapHelpers.h"
#include "Math/Vector.h"

hexmap::LayerPos hexmap::ToLayerPos(const HexPos& hexPos, const Vector2i& stride)
{
	return LayerPos(hexPos.x / stride.x, hexPos.y / stride.y);
}

hexmap::HexPos hexmap::ToHexPos(const LayerPos& hexPos, const Vector2i& stride)
{
	return HexPos(hexPos.x * stride.x, hexPos.y * stride.y);
}