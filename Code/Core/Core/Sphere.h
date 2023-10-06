#pragma once

#include <Core/Vector.h>

/// \brief A sphere in 3D space denoted by a position and radius.
class Sphere
{
public:
	constexpr Sphere() noexcept : m_Position(), m_Radius() { }
	constexpr Sphere(const Vector3f& position, const float radius) noexcept : m_Position(position), m_Radius(radius) { }

public:
	Vector3f m_Position;
	float m_Radius;
};
