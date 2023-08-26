#pragma once

#include <Core/Vector.h>

/// \brief A plane is made up of its normal and a point that lies on the plane.
class Plane
{
public:
	constexpr Plane() noexcept : m_Normal(), m_Point() { }
	constexpr Plane(const Vector3f& normal, const Vector3f& point) noexcept : m_Normal(normal), m_Point(point) { }

public:
	Vector3f m_Normal;
	Vector3f m_Point;
};

#include "AABB.inl"