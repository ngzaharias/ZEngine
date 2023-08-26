#pragma once

#include "Core/OBB.h"

#include <algorithm>

inline constexpr AABB AABB::FromExtents(const Vector3f& extents) noexcept
{
	AABB aabb;
	aabb.m_Min = -extents;
	aabb.m_Max = +extents;
	return aabb;
}

inline constexpr AABB AABB::FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept
{
	AABB aabb;
	aabb.m_Min = centre - extents;
	aabb.m_Max = centre + extents;
	return aabb;
}

inline constexpr AABB AABB::FromOBB(const OBB& obb) noexcept
{
	AABB aabb;
	aabb.m_Min.x = std::min({ obb.m_Points[0].x, obb.m_Points[1].x, obb.m_Points[2].x, obb.m_Points[3].x, obb.m_Points[4].x, obb.m_Points[5].x, obb.m_Points[6].x, obb.m_Points[7].x });
	aabb.m_Min.y = std::min({ obb.m_Points[0].y, obb.m_Points[1].y, obb.m_Points[2].y, obb.m_Points[3].y, obb.m_Points[4].y, obb.m_Points[5].y, obb.m_Points[6].y, obb.m_Points[7].y });
	aabb.m_Min.z = std::min({ obb.m_Points[0].z, obb.m_Points[1].z, obb.m_Points[2].z, obb.m_Points[3].z, obb.m_Points[4].z, obb.m_Points[5].z, obb.m_Points[6].z, obb.m_Points[7].z });
	aabb.m_Max.x = std::max({ obb.m_Points[0].x, obb.m_Points[1].x, obb.m_Points[2].x, obb.m_Points[3].x, obb.m_Points[4].x, obb.m_Points[5].x, obb.m_Points[6].x, obb.m_Points[7].x });
	aabb.m_Max.y = std::max({ obb.m_Points[0].y, obb.m_Points[1].y, obb.m_Points[2].y, obb.m_Points[3].y, obb.m_Points[4].y, obb.m_Points[5].y, obb.m_Points[6].y, obb.m_Points[7].y });
	aabb.m_Max.z = std::max({ obb.m_Points[0].z, obb.m_Points[1].z, obb.m_Points[2].z, obb.m_Points[3].z, obb.m_Points[4].z, obb.m_Points[5].z, obb.m_Points[6].z, obb.m_Points[7].z });
	return aabb;
}