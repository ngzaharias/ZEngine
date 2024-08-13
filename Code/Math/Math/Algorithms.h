#pragma once

using int32 = int32_t;
class Vector3i;

namespace enumerate
{
	inline auto Hexagonal(const int32 radius);

	inline auto Length(const float stride, const float length);

	inline auto Vector(const Vector3i& value);
	inline auto Vector(const Vector3i& min, const Vector3i& max);
}

#include "Algorithms.inl"