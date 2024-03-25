#pragma once

#include <Core/Vector.h>

/// \brief An axis aligned rectangle in 2D space using floating-point coordinates.
class Rect2f
{
public:
	constexpr Rect2f() noexcept : m_Min(), m_Max() { }
	constexpr Rect2f(const Vector2f& min, const Vector2f& max) noexcept : m_Min(min), m_Max(max) { }

	[[nodiscard]] static constexpr Rect2f FromExtents(const Vector2f& extents) noexcept;
	[[nodiscard]] static constexpr Rect2f FromExtents(const Vector2f& centre, const Vector2f& extents) noexcept;

public:
	Vector2f m_Min;
	Vector2f m_Max;
};

/// \brief An axis aligned rectangle in 2D space using integer coordinates.
class Rect2i
{
public:
	constexpr Rect2i() noexcept : m_Min(), m_Max() { }
	constexpr Rect2i(const Vector2i& min, const Vector2i& max) noexcept : m_Min(min), m_Max(max) { }

	[[nodiscard]] static constexpr Rect2i FromExtents(const Vector2i& extents) noexcept;
	[[nodiscard]] static constexpr Rect2i FromExtents(const Vector2i& centre, const Vector2i& extents) noexcept;

public:
	Vector2i m_Min;
	Vector2i m_Max;
};

#include "Rect.inl"