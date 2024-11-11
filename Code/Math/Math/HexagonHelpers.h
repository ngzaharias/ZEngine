#pragma once

#include "Math/Hexagon.h"
#include "Math/Vector.h"

namespace hexagon
{
	/// \brief Convert an axial position to an offset position.
	inline Axial ToAxial(const Offset& value);

	/// \brief Convert a world position to a hexagonal grid position.
	/// Input in the range of >= 0.f and < cellSize will all map to the same value.
	inline Axial ToAxial(const Vector2f& worldPos, const float cellSize = 1.f);

	/// \brief Convert an offset position to an axial position.
	inline Offset ToOffset(const Axial& value);

	/// \brief Convert a world position to a hexagonal grid position.
	/// Input in the range of >= 0.f and < cellSize will all map to the same value.
	inline Offset ToOffset(const Vector2f& worldPos, const float cellSize = 1.f);

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPos(const Axial& gridPos, const float cellSize = 1.f);

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPos(const Offset& gridPos, const float cellSize = 1.f);
}

#include "HexagonHelpers.inl"