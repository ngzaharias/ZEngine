#pragma once

#include "Math/AABB.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

constexpr OBB2f::OBB2f(const Vector2f& v0, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3) noexcept
{
	m_Points[0] = v0;
	m_Points[1] = v1;
	m_Points[2] = v2;
	m_Points[3] = v3;
}

constexpr OBB2f OBB2f::FromAABB(const AABB2f& aabb) noexcept
{
	OBB2f obb;
	obb.m_Points[0] = Vector2f(aabb.m_Min.x, aabb.m_Min.y);
	obb.m_Points[1] = Vector2f(aabb.m_Min.x, aabb.m_Min.y);
	obb.m_Points[2] = Vector2f(aabb.m_Max.x, aabb.m_Min.y);
	obb.m_Points[3] = Vector2f(aabb.m_Max.x, aabb.m_Min.y);
	return obb;
}

constexpr OBB2f OBB2f::FromExtents(const Vector2f& centre, const Vector2f& extents) noexcept
{
	OBB2f obb;
	obb.m_Points[0] = centre + Vector2f(-extents.x, -extents.y);
	obb.m_Points[1] = centre + Vector2f(-extents.x, -extents.y);
	obb.m_Points[2] = centre + Vector2f(+extents.x, -extents.y);
	obb.m_Points[3] = centre + Vector2f(+extents.x, -extents.y);
	return obb;
}

constexpr OBB2f OBB2f::FromExtents(const Vector2f& centre, const Matrix3x3& rotate, const Vector2f& extents) noexcept
{
	OBB2f obb;
	obb.m_Points[0] = centre + Vector2f(-extents.x, -extents.y) * rotate;
	obb.m_Points[1] = centre + Vector2f(-extents.x, -extents.y) * rotate;
	obb.m_Points[2] = centre + Vector2f(+extents.x, -extents.y) * rotate;
	obb.m_Points[3] = centre + Vector2f(+extents.x, -extents.y) * rotate;
	return obb;
}

constexpr OBB2f OBB2f::FromExtents(const Vector2f& centre, const Quaternion& rotate, const Vector2f& extents) noexcept
{
	OBB2f obb;
	obb.m_Points[0] = centre + Vector2f(-extents.x, -extents.y) * rotate;
	obb.m_Points[1] = centre + Vector2f(-extents.x, -extents.y) * rotate;
	obb.m_Points[2] = centre + Vector2f(+extents.x, -extents.y) * rotate;
	obb.m_Points[3] = centre + Vector2f(+extents.x, -extents.y) * rotate;
	return obb;
}

constexpr OBB3f::OBB3f(
	const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3,
	const Vector3f& v4, const Vector3f& v5, const Vector3f& v6, const Vector3f& v7) noexcept
{
	m_Points[0] = v0;
	m_Points[1] = v1;
	m_Points[2] = v2;
	m_Points[3] = v3;
	m_Points[4] = v4;
	m_Points[5] = v5;
	m_Points[6] = v6;
	m_Points[7] = v7;
}

constexpr OBB3f OBB3f::FromAABB(const AABB3f& aabb) noexcept
{
	OBB3f obb;
	obb.m_Points[0] = Vector3f(aabb.m_Min.x, aabb.m_Min.y, aabb.m_Min.z);
	obb.m_Points[1] = Vector3f(aabb.m_Min.x, aabb.m_Min.y, aabb.m_Max.z);
	obb.m_Points[2] = Vector3f(aabb.m_Max.x, aabb.m_Min.y, aabb.m_Max.z);
	obb.m_Points[3] = Vector3f(aabb.m_Max.x, aabb.m_Min.y, aabb.m_Min.z);
	obb.m_Points[4] = Vector3f(aabb.m_Min.x, aabb.m_Max.y, aabb.m_Min.z);
	obb.m_Points[5] = Vector3f(aabb.m_Min.x, aabb.m_Max.y, aabb.m_Max.z);
	obb.m_Points[6] = Vector3f(aabb.m_Max.x, aabb.m_Max.y, aabb.m_Max.z);
	obb.m_Points[7] = Vector3f(aabb.m_Max.x, aabb.m_Max.y, aabb.m_Min.z);
	return obb;
}

constexpr OBB3f OBB3f::FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept
{
	OBB3f obb;
	obb.m_Points[0] = centre + Vector3f(-extents.x, -extents.y, -extents.z);
	obb.m_Points[1] = centre + Vector3f(-extents.x, -extents.y, +extents.z);
	obb.m_Points[2] = centre + Vector3f(+extents.x, -extents.y, +extents.z);
	obb.m_Points[3] = centre + Vector3f(+extents.x, -extents.y, -extents.z);
	obb.m_Points[4] = centre + Vector3f(-extents.x, +extents.y, -extents.z);
	obb.m_Points[5] = centre + Vector3f(-extents.x, +extents.y, +extents.z);
	obb.m_Points[6] = centre + Vector3f(+extents.x, +extents.y, +extents.z);
	obb.m_Points[7] = centre + Vector3f(+extents.x, +extents.y, -extents.z);
	return obb;
}

constexpr OBB3f OBB3f::FromExtents(const Vector3f& centre, const Matrix3x3& rotate, const Vector3f& extents) noexcept
{
	OBB3f obb;
	obb.m_Points[0] = centre + Vector3f(-extents.x, -extents.y, -extents.z) * rotate;
	obb.m_Points[1] = centre + Vector3f(-extents.x, -extents.y, +extents.z) * rotate;
	obb.m_Points[2] = centre + Vector3f(+extents.x, -extents.y, +extents.z) * rotate;
	obb.m_Points[3] = centre + Vector3f(+extents.x, -extents.y, -extents.z) * rotate;
	obb.m_Points[4] = centre + Vector3f(-extents.x, +extents.y, -extents.z) * rotate;
	obb.m_Points[5] = centre + Vector3f(-extents.x, +extents.y, +extents.z) * rotate;
	obb.m_Points[6] = centre + Vector3f(+extents.x, +extents.y, +extents.z) * rotate;
	obb.m_Points[7] = centre + Vector3f(+extents.x, +extents.y, -extents.z) * rotate;
	return obb;
}

constexpr OBB3f OBB3f::FromExtents(const Vector3f& centre, const Quaternion& rotate, const Vector3f& extents) noexcept
{
	OBB3f obb;
	obb.m_Points[0] = centre + Vector3f(-extents.x, -extents.y, -extents.z) * rotate;
	obb.m_Points[1] = centre + Vector3f(-extents.x, -extents.y, +extents.z) * rotate;
	obb.m_Points[2] = centre + Vector3f(+extents.x, -extents.y, +extents.z) * rotate;
	obb.m_Points[3] = centre + Vector3f(+extents.x, -extents.y, -extents.z) * rotate;
	obb.m_Points[4] = centre + Vector3f(-extents.x, +extents.y, -extents.z) * rotate;
	obb.m_Points[5] = centre + Vector3f(-extents.x, +extents.y, +extents.z) * rotate;
	obb.m_Points[6] = centre + Vector3f(+extents.x, +extents.y, +extents.z) * rotate;
	obb.m_Points[7] = centre + Vector3f(+extents.x, +extents.y, -extents.z) * rotate;
	return obb;
}