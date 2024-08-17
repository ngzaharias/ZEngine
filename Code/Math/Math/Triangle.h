#pragma once

#include "Math/Vector.h"

/// \brief A triangle in 2D space using floating-point coordinates.
class Triangle2f
{
public:
	constexpr Triangle2f() noexcept 
		: m_PointA(), m_PointB(), m_PointC() { }
	constexpr Triangle2f(const Vector2f& a, const Vector2f& b, const Vector2f& c) noexcept 
		: m_PointA(a), m_PointB(b), m_PointC(c) { }

public:
	Vector2f m_PointA;
	Vector2f m_PointB;
	Vector2f m_PointC;
};

/// \brief A triangle in 3D space using floating-point coordinates.
class Triangle3f
{
public:
	constexpr Triangle3f() noexcept 
		: m_PointA(), m_PointB(), m_PointC() { }
	constexpr Triangle3f(const Vector3f& a, const Vector3f& b, const Vector3f& c) noexcept
		: m_PointA(a), m_PointB(b), m_PointC(c) { }

public:
	Vector3f m_PointA;
	Vector3f m_PointB;
	Vector3f m_PointC;
};
