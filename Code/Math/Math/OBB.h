#pragma once

#include "Math/Vector.h"

class AABB2f;
class AABB3f;
class Matrix3x3;
class Quaternion;
class Rotator;

/// \brief An orientated bounding box in 3D space denoted by 8 points.
class OBB2f
{
public:
	constexpr OBB2f() noexcept : m_Points() { }
	constexpr OBB2f(const Vector2f& v0, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3) noexcept;

	[[nodiscard]] static OBB2f FromExtents(const Rotator& rotate, const Vector2f& extents) noexcept;
	[[nodiscard]] static OBB2f FromExtents(const Vector2f& centre, const Rotator& rotate, const Vector2f& extents) noexcept;

	[[nodiscard]] static constexpr OBB2f FromAABB(const AABB2f& aabb) noexcept;
	[[nodiscard]] static constexpr OBB2f FromExtents(const Vector2f& centre, const Vector2f& extents) noexcept;
	[[nodiscard]] static constexpr OBB2f FromExtents(const Vector2f& centre, const Matrix3x3& rotate, const Vector2f& extents) noexcept;
	[[nodiscard]] static constexpr OBB2f FromExtents(const Vector2f& centre, const Quaternion& rotate, const Vector2f& extents) noexcept;

public:
	Vector2f m_Points[4];
};

/// \brief An orientated bounding box in 3D space denoted by 8 points.
class OBB3f
{
public:
	constexpr OBB3f() noexcept : m_Points() { }
	constexpr OBB3f(
		const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3,
		const Vector3f& v4, const Vector3f& v5, const Vector3f& v6, const Vector3f& v7) noexcept;

	[[nodiscard]] static OBB3f FromExtents(const Rotator& rotate, const Vector3f& extents) noexcept;
	[[nodiscard]] static OBB3f FromExtents(const Vector3f& centre, const Rotator& rotate, const Vector3f& extents) noexcept;

	[[nodiscard]] static constexpr OBB3f FromAABB(const AABB3f& aabb) noexcept;
	[[nodiscard]] static constexpr OBB3f FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr OBB3f FromExtents(const Vector3f& centre, const Matrix3x3& rotate, const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr OBB3f FromExtents(const Vector3f& centre, const Quaternion& rotate, const Vector3f& extents) noexcept;

public:
	Vector3f m_Points[8];
};

#include "OBB.inl"