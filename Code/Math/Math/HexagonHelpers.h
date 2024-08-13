#pragma once

#include "Math/Vector.h"

namespace hexagon
{
	/// \brief Convert a world position to a hexagonal grid position.
	/// Input in the range of >= 0.f and < cellSize will all map to the same value.
	inline Vector2i ToGridPos(const Vector2f& worldPos, const float cellSize = 1.f);

	/// \brief Convert a hexagonal grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPos(const Vector2i& hexPos, const float cellSize = 1.f);
}

#include "HexagonHelpers.inl"