#pragma once

#include "Math/Ray.h"
#include "Math/Segment.h"

Ray2f Ray2f::FromPoints(const Vector2f& pointA, const Vector2f& pointB)
{
	Ray2f ray;
	ray.m_Position = pointA;
	ray.m_Direction = (pointB - pointA).Normalized();
	return ray;
}

Ray2f Ray2f::FromSegment(const Segment2f& segment)
{
	Ray2f ray;
	ray.m_Position = segment.m_PointA;
	ray.m_Direction = (segment.m_PointB - segment.m_PointA).Normalized();
	return ray;
}

Ray3f Ray3f::FromPoints(const Vector3f& pointA, const Vector3f& pointB)
{
	Ray3f ray;
	ray.m_Position = pointA;
	ray.m_Direction = (pointB - pointA).Normalized();
	return ray;
}

Ray3f Ray3f::FromSegment(const Segment3f& segment)
{
	Ray3f ray;
	ray.m_Position = segment.m_PointA;
	ray.m_Direction = (segment.m_PointB - segment.m_PointA).Normalized();
	return ray;
}
