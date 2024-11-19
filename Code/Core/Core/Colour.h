#pragma once

#include "Math/Vector.h"

class Colour
{
public:
	constexpr Colour() noexcept : m_Value() { }
	constexpr Colour(const Vector4f& value) noexcept : m_Value(value) { }

public:
	Vector4f m_Value;
};