#pragma once

#include "Math/Math.h"
#include "Math/Vector.h"

// https://www.redblobgames.com/grids/hexagons/

namespace _private
{
	inline hexagon::Axial Round(const Vector2f& value)
	{
		float x = value.x;
		float y = value.y;
		const float q = math::Round(x);
		const float r = math::Round(y);
		x -= q;
		y -= r;

		const float dx = math::Round(x + 0.5f * y) * (x * x >= y * y);
		const float dy = math::Round(y + 0.5f * x) * (x * x < y* y);
		return hexagon::Axial(static_cast<int32>(q + dx), static_cast<int32>(r + dy));
	}
}

inline hexagon::Axial hexagon::ToAxial(const Offset& value)
{
	// odd-q
	const int32 q = value.col;
	const int32 r = value.row - (value.col - (value.col & 1)) / 2;
	return Axial(q, r);
}

inline hexagon::Axial hexagon::ToAxial(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	constexpr float f2d3 = 2.f / 3.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d3 = SQUARE_ROOT_THREE / 3.f;

	const float q = (f2d3 * worldPos.x) / cellSize;
	const float r = (-worldPos.x / 3.f + sqrt3d3 * worldPos.y) / cellSize;
	return _private::Round(Vector2f(q, r));
}

inline hexagon::Offset hexagon::ToOffset(const Axial& value)
{
	const int32 col = value.q;
	const int32 row = value.r + (value.q - (value.q & 1)) / 2;
	return Offset(col, row);
}

inline hexagon::Offset hexagon::ToOffset(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	constexpr float f2d3 = 2.f / 3.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d3 = SQUARE_ROOT_THREE / 3.f;

	const float q = (f2d3 * worldPos.x) / cellSize;
	const float r = (-worldPos.x / 3.f + sqrt3d3 * worldPos.y) / cellSize;
	const Axial gridPos = _private::Round(Vector2f(q, r));
	return ToOffset(gridPos);
}

inline Vector2f hexagon::ToWorldPos(const Axial& gridPos, const float cellSize /*= 1.f*/)
{
	constexpr float f3d2 = 3.f / 2.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d2 = SQUARE_ROOT_THREE / 2.f;

	const float x = (sqrt3 * gridPos.q + sqrt3d2 * gridPos.r) * cellSize;
	const float y = (f3d2 * gridPos.r) * cellSize;
	return Vector2f(x, y);
}

inline Vector2f hexagon::ToWorldPos(const Offset& gridPos, const float cellSize /*= 1.f*/)
{
	// odd-q
	constexpr float sqrt3 = SQUARE_ROOT_THREE;

	const float x = cellSize * 3.f / 2.f * gridPos.col;
	const float y = cellSize * sqrt3 * (gridPos.row + 0.5f * (gridPos.col & 1));
	return Vector2f(x, y);
}