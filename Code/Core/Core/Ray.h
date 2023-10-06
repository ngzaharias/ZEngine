#pragma once

#include <Core/Vector.h>

class Line;

/// \brief A ray in 3D space denoted by a origin and direction.
class Ray
{
public:
	constexpr Ray() noexcept : m_Direction(), m_OriginPos() { }
	constexpr Ray(const Vector3f& origin, const Vector3f& direction) noexcept : m_OriginPos(origin), m_Direction(direction) { }

	[[nodiscard]] static Ray FromLine(const Line& line);
	[[nodiscard]] static Ray FromPoints(const Vector3f& pointA, const Vector3f& pointB);

public:
	Vector3f m_OriginPos;
	Vector3f m_Direction;
};
