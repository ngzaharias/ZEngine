#pragma once

#include "Math/Vector.h"

/// \brief A plane in 3D space denoted by its normal and a point that lies on the plane.
class Plane3f
{
public:
	constexpr Plane3f() noexcept : m_Normal(), m_Point() { }
	constexpr Plane3f(const Vector3f& normal, const Vector3f& point) noexcept : m_Normal(normal), m_Point(point) { }

public:
	Vector3f m_Normal;
	Vector3f m_Point;
};
