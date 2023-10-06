#pragma once

#include <Core/Vector.h>

class OBB;

/// \brief An axis aligned bounding box in 3D space denoted by a min and max.
class AABB
{
public:
	constexpr AABB() noexcept : m_Min(), m_Max() { }
	constexpr AABB(const Vector3f& min, const Vector3f& max) noexcept : m_Min(min), m_Max(max) { }

	[[nodiscard]] static constexpr AABB FromExtents(const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr AABB FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept;
	[[nodiscard]] static constexpr AABB FromOBB(const OBB& obb) noexcept;

public:
	Vector3f m_Min;
	Vector3f m_Max;
};

#include "AABB.inl"