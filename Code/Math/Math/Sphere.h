#pragma once

#include "Math/Vector.h"

/// \brief A sphere in 3D space using floating-point coordinates.
class Sphere3f
{
public:
	constexpr Sphere3f() noexcept : m_Position(), m_Radius() { }
	constexpr Sphere3f(const float radius) noexcept : m_Position(Vector3f::Zero), m_Radius(radius) { }
	constexpr Sphere3f(const Vector3f& position, const float radius) noexcept : m_Position(position), m_Radius(radius) { }

public:
	Vector3f m_Position;
	float m_Radius;
};