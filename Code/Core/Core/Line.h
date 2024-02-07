#pragma once

#include <Core/Vector.h>

/// \brief A line in 3D space denoted by two points that extends in opposite directions infinitely.
class Line
{
public:
	constexpr Line() noexcept : m_PointA(), m_PointB() { }
	constexpr Line(const Vector3f& a, const Vector3f& b) noexcept : m_PointA(a), m_PointB(b) { }

public:
	Vector3f m_PointA;
	Vector3f m_PointB;
};
