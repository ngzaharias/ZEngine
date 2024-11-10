#pragma once

#include "Math/Math.h"
#include "Math/Vector.h"

// https://www.redblobgames.com/grids/hexagons/

namespace _private
{
	inline Vector2f Round_Axial(const Vector2f& value)
	{
		float x = value.x;
		float y = value.y;
		const float q = math::Round(x);
		const float r = math::Round(y);
		x -= q;
		y -= r;

		const float dx = math::Round(x + 0.5f * y) * (x * x >= y * y);
		const float dy = math::Round(y + 0.5f * x) * (x * x < y* y);
		return Vector2f(q + dx, r + dy);
	}
}

inline Vector2i hexagon::ToGridPosH_Axial(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	constexpr float f2d3 = 2.f / 3.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d3 = SQUARE_ROOT_THREE / 3.f;

	const float q = (f2d3 * worldPos.x) / cellSize;
	const float r = (-worldPos.x / 3.f + sqrt3d3 * worldPos.y) / cellSize;
	const Vector2f hexPos = _private::Round_Axial(Vector2f(q, r));
	return Vector2i(int32(hexPos.x), int32(hexPos.y));
}

inline Vector2i hexagon::ToGridPosV_Axial(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	constexpr float f2d3 = 2.f / 3.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d3 = SQUARE_ROOT_THREE / 3.f;

	const float q = (sqrt3d3 * worldPos.x + -worldPos.y / 3.f) / cellSize;
	const float r = (f2d3 * worldPos.y) / cellSize;
	const Vector2f hexPos = _private::Round_Axial(Vector2f(q, r));
	return Vector2i(int32(hexPos.x), int32(hexPos.y));
}

inline Vector2f hexagon::ToWorldPosH_Axial(const Vector2i& hexPos, const float cellSize /*= 1.f*/)
{
	constexpr float f3d2 = 3.f / 2.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d2 = SQUARE_ROOT_THREE / 2.f;

	const float x = (f3d2 * hexPos.x) * cellSize;
	const float y = (sqrt3d2 * hexPos.x + sqrt3 * hexPos.y) * cellSize;
	return Vector2f(x, y);
}

inline Vector2f hexagon::ToWorldPosH_Offset(const Vector2i& hexPos, const float cellSize /*= 1.f*/)
{
	constexpr float sqrt3 = SQUARE_ROOT_THREE;

	const float x = cellSize * 3.f / 2.f * hexPos.x;
	const float y = cellSize * sqrt3 * (hexPos.y + 0.5f * (hexPos.x & 1));
	return Vector2f(x, y);
}

inline Vector2f hexagon::ToWorldPosV_Axial(const Vector2i& hexPos, const float cellSize /*= 1.f*/)
{
	constexpr float f3d2 = 3.f / 2.f;
	constexpr float sqrt3 = SQUARE_ROOT_THREE;
	constexpr float sqrt3d2 = SQUARE_ROOT_THREE / 2.f;

	const float x = (sqrt3 * hexPos.x + sqrt3d2 * hexPos.y) * cellSize;
	const float y = (f3d2 * hexPos.y) * cellSize;
	return Vector2f(x, y);
}

inline Vector2f hexagon::ToWorldPosV_Offset(const Vector2i& hexPos, const float cellSize /*= 1.f*/)
{
	constexpr float sqrt3 = SQUARE_ROOT_THREE;

	const float x = cellSize * sqrt3 * (hexPos.x + 0.5f * (hexPos.y & 1));
	const float y = cellSize * 3.f / 2.f * hexPos.y;
	return Vector2f(x, y);
}