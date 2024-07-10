#pragma once

#include "Math/Vector.h"

/// \brief A line in 3D space bounded by two points.
class Segment2f
{
public:
	constexpr Segment2f() noexcept : m_PointA(), m_PointB() { }
	constexpr Segment2f(const Vector2f& a, const Vector2f& b) noexcept : m_PointA(a), m_PointB(b) { }

public:
	Vector2f m_PointA;
	Vector2f m_PointB;
};

/// \brief A line in 3D space bounded by two points.
class Segment3f
{
public:
	constexpr Segment3f() noexcept : m_PointA(), m_PointB() { }
	constexpr Segment3f(const Vector3f& a, const Vector3f& b) noexcept : m_PointA(a), m_PointB(b) { }

public:
	Vector3f m_PointA;
	Vector3f m_PointB;
};
