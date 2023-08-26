#pragma once

#include <Core/Vector.h>

class Circle
{
public:
	constexpr Circle() noexcept : m_Position(), m_Radius() { }
	constexpr Circle(const Vector3f& position, const float radius) noexcept : m_Position(position), m_Radius(radius) { }

public:
	Vector3f m_Position;
	float m_Radius;
};
