#pragma once

#include "Math/Vector.h"

/// \brief A circle in 2D space using floating-point coordinates.
class Circle2f
{
public:
	constexpr Circle2f() noexcept : m_Position(), m_Radius() { }
	constexpr Circle2f(const float radius) noexcept : m_Position(Vector2f::Zero), m_Radius(radius) { }
	constexpr Circle2f(const Vector2f& position, const float radius) noexcept : m_Position(position), m_Radius(radius) { }

public:
	Vector2f m_Position;
	float m_Radius;
};