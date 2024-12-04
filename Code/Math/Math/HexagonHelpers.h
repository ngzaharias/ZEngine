#pragma once

#include "Math/AABB.h"
#include "Math/Hexagon.h"
#include "Math/Vector.h"

namespace hexagon
{
	inline AABB2f ToAABB(const Vector2i& count, const float radius);
	inline Vector2f ToSize(const Vector2i& count, const float radius);

	/// \brief Convert an axial position to an offset position.
	inline Axial ToAxial(const Offset& value);

	/// \brief Convert a world position to a hexagonal grid position.
	/// Input in the range of >= 0.f and < radius will all map to the same value.
	inline Axial ToAxial(const Vector2f& worldPos, const float radius = 1.f);

	/// \brief Convert an offset position to an axial position.
	inline Offset ToOffset(const Axial& value);

	/// \brief Convert a world position to a hexagonal grid position.
	/// Input in the range of >= 0.f and < radius will all map to the same value.
	inline Offset ToOffset(const Vector2f& worldPos, const float radius = 1.f);

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPos(const Axial& gridPos, const float radius = 1.f);

	/// \brief Convert a grid position to a world position.
	/// The world position will be centered on the grid position.
	inline Vector2f ToWorldPos(const Offset& gridPos, const float radius = 1.f);
}

#include "HexagonHelpers.inl"