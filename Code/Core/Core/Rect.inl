#pragma once

#include <algorithm>

inline constexpr Rect2f Rect2f::FromExtents(const Vector2f& extents) noexcept
{
	Rect2f rect;
	rect.m_Min = -extents;
	rect.m_Max = +extents;
	return rect;
}

inline constexpr Rect2f Rect2f::FromExtents(const Vector2f& centre, const Vector2f& extents) noexcept
{
	Rect2f rect;
	rect.m_Min = centre - extents;
	rect.m_Max = centre + extents;
	return rect;
}

inline constexpr Rect2i Rect2i::FromExtents(const Vector2i& extents) noexcept
{
	Rect2i rect;
	rect.m_Min = -extents;
	rect.m_Max = +extents;
	return rect;
}

inline constexpr Rect2i Rect2i::FromExtents(const Vector2i& centre, const Vector2i& extents) noexcept
{
	Rect2i rect;
	rect.m_Min = centre - extents;
	rect.m_Max = centre + extents;
	return rect;
}