#pragma once

#include <Core/Vector.h>

class AABB;
class Matrix3x3;
class Quaternion;
class Rotator;

class OBB
{
public:
	constexpr OBB() noexcept : m_Points() { }
	constexpr OBB(
		const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3,
		const Vector3f& v4, const Vector3f& v5, const Vector3f& v6, const Vector3f& v7) noexcept;

	[[nodiscard]] static OBB FromExtents(const Rotator& rotate, const Vector3f& extents) noexcept;
	[[nodiscard]] static OBB FromExtents(const Vector3f& centre, const Rotator& rotate, const Vector3f& extents) noexcept;

	[[nodiscard]] static constexpr OBB FromAABB(const AABB& aabb) noexcept;
	[[nodiscard]] static constexpr OBB FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr OBB FromExtents(const Vector3f& centre, const Matrix3x3& rotate, const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr OBB FromExtents(const Vector3f& centre, const Quaternion& rotate, const Vector3f& extents) noexcept;

public:
	Vector3f m_Points[8];
};

#include "OBB.inl"