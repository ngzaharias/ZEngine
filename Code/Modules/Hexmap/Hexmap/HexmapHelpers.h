#pragma once

class Vector2i;

namespace hexmap
{
	class HexPos;
	class LayerPos;

	LayerPos ToLayerPos(const HexPos& hexPos, const Vector2i& stride);
	HexPos ToHexPos(const LayerPos& hexPos, const Vector2i& stride);
}