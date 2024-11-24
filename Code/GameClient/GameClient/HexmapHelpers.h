#pragma once

#include "GameClient/HexmapData.h"
#include "Math/Vector.h"

namespace hexmap
{
	LayerPos ToLayerPos(const HexPos& hexPos, const Vector2i& stride);
	HexPos ToHexPos(const LayerPos& hexPos, const Vector2i& stride);
}