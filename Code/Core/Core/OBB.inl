#pragma once

#include "Core/AABB.h"
#include "Core/Matrix.h"
#include "Core/Quaternion.h"

constexpr OBB::OBB(
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

constexpr OBB OBB::FromAABB(const AABB& aabb) noexcept
{
	OBB obb;
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

constexpr OBB OBB::FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept
{
	OBB obb;
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

constexpr OBB OBB::FromExtents(const Vector3f& centre, const Matrix3x3& rotate, const Vector3f& extents) noexcept
{
	OBB obb;
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

constexpr OBB OBB::FromExtents(const Vector3f& centre, const Quaternion& rotate, const Vector3f& extents) noexcept
{
	OBB obb;
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