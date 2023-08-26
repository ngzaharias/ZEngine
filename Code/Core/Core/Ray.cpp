#pragma once

#include "Core/Line.h"
#include "Core/Ray.h"

Ray Ray::FromLine(const Line& line)
{
	Ray ray;
	ray.m_OriginPos = line.m_PointA;
	ray.m_Direction = (line.m_PointB - line.m_PointA).Normalized();
	return ray;
}


Ray Ray::FromPoints(const Vector3f& pointA, const Vector3f& pointB)
{
	Ray ray;
	ray.m_OriginPos = pointA;
	ray.m_Direction = (pointB - pointA).Normalized();
	return ray;
}
