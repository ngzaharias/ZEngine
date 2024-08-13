#pragma once

#include "Math/Math.h"
#include "Math/Vector.h"

namespace _private
{
	Vector2f Round(const Vector2f& value)
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

inline Vector2i hexagon::ToGridPos(const Vector2f& worldPos, const float cellSize /*= 1.f*/)
{
	const float q = (2.f / 3.f * worldPos.x) / cellSize;
	const float r = (-1.f / 3.f * worldPos.x + math::Sqrt(3.f) / 3.f * worldPos.y) / cellSize;
	const Vector2f hexPos = _private::Round(Vector2f(q, r));
	return Vector2i(int32(hexPos.x), int32(hexPos.y));
}

inline Vector2f hexagon::ToWorldPos(const Vector2i& hexPos, const float cellSize /*= 1.f*/)
{
	const float x = (3.f / 2.f * hexPos.x) * cellSize;
	const float y = (math::Sqrt(3.f) / 2.f * hexPos.x + math::Sqrt(3.f) * hexPos.y) * cellSize;
	return Vector2f(x, y);
}