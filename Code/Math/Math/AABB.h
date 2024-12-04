#pragma once

#include "Math/Vector.h"

class OBB3f;

/// \brief An axis aligned rectangle in 2D space using floating-point coordinates.
class AABB2f
{
public:
	constexpr AABB2f() noexcept : m_Min(), m_Max() { }
	constexpr AABB2f(const Vector2f& min, const Vector2f& max) noexcept : m_Min(min), m_Max(max) { }

	[[nodiscard]] static constexpr AABB2f FromExtents(const Vector2f& extents) noexcept;
	[[nodiscard]] static constexpr AABB2f FromExtents(const Vector2f& centre, const Vector2f& extents) noexcept;

public:
	Vector2f m_Min;
	Vector2f m_Max;
};

/// \brief An axis aligned rectangle in 2D space using integer coordinates.
class AABB2i
{
public:
	constexpr AABB2i() noexcept : m_Min(), m_Max() { }
	constexpr AABB2i(const Vector2i& min, const Vector2i& max) noexcept : m_Min(min), m_Max(max) { }

	[[nodiscard]] static constexpr AABB2i FromExtents(const Vector2i& extents) noexcept;
	[[nodiscard]] static constexpr AABB2i FromExtents(const Vector2i& centre, const Vector2i& extents) noexcept;

public:
	Vector2i m_Min;
	Vector2i m_Max;
};

/// \brief An axis aligned bounding box in 3D space using floating-point coordinates.
class AABB3f
{
public:
	constexpr AABB3f() noexcept : m_Min(), m_Max() { }
	constexpr AABB3f(const Vector3f& min, const Vector3f& max) noexcept : m_Min(min), m_Max(max) { }

	[[nodiscard]] static constexpr AABB3f FromExtents(const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr AABB3f FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr AABB3f FromOBB(const OBB3f& obb) noexcept;

public:
	Vector3f m_Min;
	Vector3f m_Max;
};

#include "AABB.inl"