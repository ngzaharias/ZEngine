#pragma once

#include "Core/Ray.h"
#include "Core/Segment.h"

Ray Ray::FromPoints(const Vector3f& pointA, const Vector3f& pointB)
{
	Ray ray;
	ray.m_OriginPos = pointA;
	ray.m_Direction = (pointB - pointA).Normalized();
	return ray;
}

Ray Ray::FromSegment(const Segment& segment)
{
	Ray ray;
	ray.m_OriginPos = segment.m_PointA;
	ray.m_Direction = (segment.m_PointB - segment.m_PointA).Normalized();
	return ray;
}
