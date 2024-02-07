#pragma once

#include <Core/Vector.h>

/// \brief A line in 3D space bounded by two points.
class Segment
{
public:
	constexpr Segment() noexcept : m_PointA(), m_PointB() { }
	constexpr Segment(const Vector3f& a, const Vector3f& b) noexcept : m_PointA(a), m_PointB(b) { }

public:
	Vector3f m_PointA;
	Vector3f m_PointB;
};
