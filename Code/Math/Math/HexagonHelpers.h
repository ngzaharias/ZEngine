#pragma once

#include "Math/Vector.h"

namespace hexagon
{
	/// \brief Convert a world position to an axial horizontal hexagonal grid position.
	/// Input in the range of >= 0.f and < cellSize will all map to the same value.
	inline Vector2i ToGridPosH_Axial(const Vector2f& worldPos, const float cellSize = 1.f);

	/// \brief Convert a world position to an axial vertical hexagonal grid position.
	/// Input in the range of >= 0.f and < cellSize will all map to the same value.
	inline Vector2i ToGridPosV_Axial(const Vector2f& worldPos, const float cellSize = 1.f);

	/// \brief Convert an axial horizontal hexagonal grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPosH_Axial(const Vector2i& hexPos, const float cellSize = 1.f);

	/// \brief Convert an offset horizontal hexagonal grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPosH_Offset(const Vector2i& hexPos, const float cellSize = 1.f);

	/// \brief Convert an axial vertical hexagonal grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPosV_Axial(const Vector2i& hexPos, const float cellSize = 1.f);

	/// \brief Convert a offset vertical hexagonal grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPosV_Offset(const Vector2i& hexPos, const float cellSize = 1.f);
}

#include "HexagonHelpers.inl"