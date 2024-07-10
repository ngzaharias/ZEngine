#pragma once

#include "Math/OBB.h"

#include <algorithm>

inline constexpr AABB2f AABB2f::FromExtents(const Vector2f& extents) noexcept
{
	AABB2f rect;
	rect.m_Min = -extents;
	rect.m_Max = +extents;
	return rect;
}

inline constexpr AABB2f AABB2f::FromExtents(const Vector2f& centre, const Vector2f& extents) noexcept
{
	AABB2f rect;
	rect.m_Min = centre - extents;
	rect.m_Max = centre + extents;
	return rect;
}

inline constexpr AABB2i AABB2i::FromExtents(const Vector2i& extents) noexcept
{
	AABB2i rect;
	rect.m_Min = -extents;
	rect.m_Max = +extents;
	return rect;
}

inline constexpr AABB2i AABB2i::FromExtents(const Vector2i& centre, const Vector2i& extents) noexcept
{
	AABB2i rect;
	rect.m_Min = centre - extents;
	rect.m_Max = centre + extents;
	return rect;
}

inline constexpr AABB3f AABB3f::FromExtents(const Vector3f& extents) noexcept
{
	AABB3f aabb;
	aabb.m_Min = -extents;
	aabb.m_Max = +extents;
	return aabb;
}

inline constexpr AABB3f AABB3f::FromExtents(const Vector3f& centre, const Vector3f& extents) noexcept
{
	AABB3f aabb;
	aabb.m_Min = centre - extents;
	aabb.m_Max = centre + extents;
	return aabb;
}

inline constexpr AABB3f AABB3f::FromOBB(const OBB3f& obb) noexcept
{
	AABB3f aabb;
	aabb.m_Min.x = std::min({ obb.m_Points[0].x, obb.m_Points[1].x, obb.m_Points[2].x, obb.m_Points[3].x, obb.m_Points[4].x, obb.m_Points[5].x, obb.m_Points[6].x, obb.m_Points[7].x });
	aabb.m_Min.y = std::min({ obb.m_Points[0].y, obb.m_Points[1].y, obb.m_Points[2].y, obb.m_Points[3].y, obb.m_Points[4].y, obb.m_Points[5].y, obb.m_Points[6].y, obb.m_Points[7].y });
	aabb.m_Min.z = std::min({ obb.m_Points[0].z, obb.m_Points[1].z, obb.m_Points[2].z, obb.m_Points[3].z, obb.m_Points[4].z, obb.m_Points[5].z, obb.m_Points[6].z, obb.m_Points[7].z });
	aabb.m_Max.x = std::max({ obb.m_Points[0].x, obb.m_Points[1].x, obb.m_Points[2].x, obb.m_Points[3].x, obb.m_Points[4].x, obb.m_Points[5].x, obb.m_Points[6].x, obb.m_Points[7].x });
	aabb.m_Max.y = std::max({ obb.m_Points[0].y, obb.m_Points[1].y, obb.m_Points[2].y, obb.m_Points[3].y, obb.m_Points[4].y, obb.m_Points[5].y, obb.m_Points[6].y, obb.m_Points[7].y });
	aabb.m_Max.z = std::max({ obb.m_Points[0].z, obb.m_Points[1].z, obb.m_Points[2].z, obb.m_Points[3].z, obb.m_Points[4].z, obb.m_Points[5].z, obb.m_Points[6].z, obb.m_Points[7].z });
	return aabb;
}