#pragma once

#include "Math/Vector.h"

/// \brief A line in 3D space denoted by two points that extends in opposite directions infinitely.
class Line2f
{
public:
	constexpr Line2f() noexcept : m_PointA(), m_PointB() { }
	constexpr Line2f(const Vector2f& a, const Vector2f& b) noexcept : m_PointA(a), m_PointB(b) { }

public:
	Vector2f m_PointA;
	Vector2f m_PointB;
};


/// \brief A line in 3D space denoted by two points that extends in opposite directions infinitely.
class Line3f
{
public:
	constexpr Line3f() noexcept : m_PointA(), m_PointB() { }
	constexpr Line3f(const Vector3f& a, const Vector3f& b) noexcept : m_PointA(a), m_PointB(b) { }

public:
	Vector3f m_PointA;
	Vector3f m_PointB;
};
