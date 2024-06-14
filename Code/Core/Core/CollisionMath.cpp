#include "CollisionMath.h"

#include "Core/AABB.h"
#include "Core/Circle.h"
#include "Core/Line.h"
#include "Core/OBB.h"
#include "Core/Plane.h"
#include "Core/Ray.h"
#include "Core/Rect.h"
#include "Core/Segment.h"
#include "Core/Sphere.h"
#include "Core/Triangle.h"
#include "Core/Vector.h"
#include "Core/VectorMath.h"

#include <algorithm>
#include <float.h>

namespace
{
	bool AreCounterClockwiseOrdered(const Vector2f& a, const Vector2f& b, const Vector2f& c)
	{
		return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
	}
}

bool math::Intersection(const Line3f& line, const Plane3f& plane, Vector3f& out_IntersectPos)
{
	const Vector3f v = line.m_PointB - line.m_PointA;
	const float d0 = math::Dot(v, plane.m_Normal);
	if (d0 == 0.f)
		return false;

	const Vector3f w = line.m_PointA - plane.m_Point;
	const float d1 = math::Dot(w, plane.m_Normal);
	const float d2 = d1 / d0;
	out_IntersectPos = line.m_PointA - v * d2;
	return true;
}

// https://github.com/BSVino/MathForGameDevelopers/blob/line-plane-intersection/math/collision.cpp
bool math::Intersection(const Ray3f& ray, const Plane3f& plane, Vector3f& out_IntersectPos)
{
	const float d0 = math::Dot(ray.m_Direction, plane.m_Normal);
	if (d0 == 0.f)
		return false;

	const Vector3f w = plane.m_Point - ray.m_Position;
	const float d1 = math::Dot(w, plane.m_Normal);
	const float d2 = d1 / d0;
	out_IntersectPos = ray.m_Position + ray.m_Direction * d2;
	return d2 >= 0.f;
}

// https://github.com/BSVino/MathForGameDevelopers/blob/line-plane-intersection/math/collision.cpp
bool math::Intersection(const Segment3f& segment, const Plane3f& plane, Vector3f& out_IntersectPos)
{
	const Vector3f v = segment.m_PointB - segment.m_PointA;
	const float d0 = math::Dot(v, plane.m_Normal);
	if (d0 == 0.f)
		return false;

	const Vector3f w = plane.m_Point - segment.m_PointA;
	const float d1 = math::Dot(w, plane.m_Normal);
	const float d2 = d1 / d0;
	out_IntersectPos = segment.m_PointA + v * d2;
	return d2 >= 0.f && d2 <= 1.f;
}

bool math::IsOverlapping(const Circle2f& a, const Circle2f& b)
{
	const float distanceSqr = math::DistanceSqr(a.m_Position, b.m_Position);
	const float radiiSqr = math::Sqr(a.m_Radius + b.m_Radius);
	return distanceSqr <= radiiSqr;
}

bool math::IsOverlapping(const Circle2f& a, const Line2f& b)
{
	const Vector2f intersectPos = math::Project(a.m_Position, b);
	const float intersectSqr = math::DistanceSqr(a.m_Position, intersectPos);
	return intersectSqr <= math::Sqr(a.m_Radius);
}

bool math::IsOverlapping(const Circle2f& a, const Ray2f& b)
{
	const Vector2f intersectPos = math::Project(a.m_Position, b);
	const float intersectSqr = math::DistanceSqr(a.m_Position, intersectPos);
	return intersectSqr <= math::Sqr(a.m_Radius);
}

// https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
bool math::IsOverlapping(const Circle2f& a, const Rect2f& b)
{
	const float halfW = (b.m_Max.x - b.m_Min.x) * 0.5f;
	const float distX = std::abs(a.m_Position.x - (b.m_Min.x + halfW));
	if (distX > (halfW + a.m_Radius))
		return false;

	const float halfH = (b.m_Max.y - b.m_Min.y) * 0.5f;
	const float distY = std::abs(a.m_Position.y - (b.m_Min.y + halfW));
	if (distY > (halfW + a.m_Radius))
		return false;

	if (distX <= halfW)
		return true;
	if (distY <= halfH)
		return true;

	const float dSqr = math::Sqr(distX - halfW) + math::Sqr(distY - halfH);
	return dSqr <= math::Sqr(a.m_Radius);
}

bool math::IsOverlapping(const Circle2f& a, const Segment2f& b)
{
	const Vector2f intersectPos = math::Project(a.m_Position, b);
	const float intersectSqr = math::DistanceSqr(a.m_Position, intersectPos);
	return intersectSqr <= math::Sqr(a.m_Radius);
}

bool math::IsOverlapping(const Circle2f& a, const Triangle2f& b)
{
	return false;
}

bool math::IsOverlapping(const Circle2f& a, const Vector2f& b)
{
	const float distanceSqr = math::DistanceSqr(a.m_Position, b);
	const float radiiSqr = math::Sqr(a.m_Radius);
	return distanceSqr <= radiiSqr;
}

bool math::IsOverlapping(const Line2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Line2f& a, const Line2f& b)
{
	return math::Cross(a.m_PointB - a.m_PointA, b.m_PointB - b.m_PointA) != 0.f;
}

// https://stackoverflow.com/questions/14307158/how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
bool math::IsOverlapping(const Line2f& a, const Ray2f& b)
{
	const Vector2f v1 = a.m_PointB - a.m_PointA;
	const Vector2f v2 = math::Perpendicular(b.m_Direction);
	const float d0 = math::Dot(v1, v2);
	if (d0 == 0.f)
		return false;

	const Vector2f w = b.m_Position - a.m_PointA;
	const float d1 = math::Cross(v1, w) / d0;
	const float d2 = math::Dot(w, v2) / d0;
	return d1 >= 0.f && d2 >= 0.f;
}

// https://stackoverflow.com/questions/99353/how-to-test-if-a-line-segment-intersects-an-axis-aligned-rectange-in-2d
// https://stackoverflow.com/questions/3746274/line-intersection-with-aabb-rectangle
bool math::IsOverlapping(const Line2f& a, const Rect2f& b)
{
	if (math::IsOverlapping(a, Segment2f(b.m_Min, Vector2f(b.m_Min.x, b.m_Max.y))))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_Min, Vector2f(b.m_Max.x, b.m_Min.y))))
		return true;
	if (math::IsOverlapping(a, Segment2f(Vector2f(b.m_Min.x, b.m_Max.y), b.m_Max)))
		return true;
	if (math::IsOverlapping(a, Segment2f(Vector2f(b.m_Max.x, b.m_Min.y), b.m_Max)))
		return true;
	return false;
}

// https://stackoverflow.com/questions/14307158/how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
bool math::IsOverlapping(const Line2f& a, const Segment2f& b)
{
	const Vector2f v1 = b.m_PointB - b.m_PointA;
	const Vector2f v2 = math::Perpendicular(a.m_PointB - a.m_PointA);
	const float d0 = math::Dot(v1, v2);
	if (d0 == 0.f)
		return false;

	const Vector2f w = a.m_PointA - b.m_PointA;
	const float t1 = math::Cross(v1, w) / d0;
	const float t2 = math::Dot(w, v2) / d0;
	return t2 >= 0.f && t2 <= 1.f;
}

bool math::IsOverlapping(const Line2f& a, const Triangle2f& b)
{
	if (math::IsOverlapping(a, Segment2f(b.m_PointA, b.m_PointB)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointB, b.m_PointC)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointC, b.m_PointA)))
		return true;
	return false;
}

bool math::IsOverlapping(const Ray2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Ray2f& a, const Line2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Ray2f& a, const Ray2f& b)
{
	const float det = math::Cross(b.m_Direction, a.m_Direction);
	if (det == 0.f)
		return false;

	const Vector2f w = b.m_Position - a.m_Position;
	const float u = (w.y * b.m_Direction.x - w.x * b.m_Direction.y) / det;
	const float v = (w.y * a.m_Direction.x - w.x * a.m_Direction.y) / det;
	return u >= 0.f && v >= 0.f;
}

bool math::IsOverlapping(const Ray2f& a, const Rect2f& b)
{
	const Vector2f reciprocal = Vector2f(
		1.f / a.m_Direction.x, 
		1.f / a.m_Direction.y);

	const float t1 = (b.m_Min.x - a.m_Position.x) * reciprocal.x;
	const float t2 = (b.m_Max.x - a.m_Position.x) * reciprocal.x;
	const float t3 = (b.m_Min.y - a.m_Position.y) * reciprocal.y;
	const float t4 = (b.m_Max.y - a.m_Position.y) * reciprocal.y;

	const float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
	const float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

	if (tmax < 0.f)
		return false;
	if (tmin > tmax)
		return false;
	return true;
}

// https://stackoverflow.com/questions/14307158/how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
bool math::IsOverlapping(const Ray2f& a, const Segment2f& b)
{
	const Vector2f v1 = b.m_PointB - b.m_PointA;
	const Vector2f v2 = math::Perpendicular(a.m_Direction);
	const float d0 = math::Dot(v1, v2);
	if (d0 == 0.f)
		return false;

	const Vector2f w = a.m_Position - b.m_PointA;
	const float t1 = math::Cross(v1, w) / d0;
	const float t2 = math::Dot(w, v2) / d0;
	return t1 >= 0.f && (t2 >= 0.f && t2 <= 1.f);
}

bool math::IsOverlapping(const Ray2f& a, const Triangle2f& b)
{
	if (math::IsOverlapping(a, Segment2f(b.m_PointA, b.m_PointB)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointB, b.m_PointC)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointC, b.m_PointA)))
		return true;
	return false;
}

bool math::IsOverlapping(const Rect2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Rect2f& a, const Line2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Rect2f& a, const Ray2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Rect2f& a, const Rect2f& b)
{
	return (a.m_Min.x <= b.m_Max.x && a.m_Max.x >= b.m_Min.x)
		&& (a.m_Min.y <= b.m_Max.y && a.m_Max.y >= b.m_Min.y);
}

bool math::IsOverlapping(const Rect2f& a, const Segment2f& b)
{
	const Vector2f& a1 = a.m_Min;
	const Vector2f& a3 = a.m_Max;
	const Vector2f a2 = Vector2f(a.m_Min.x, a.m_Max.y);
	const Vector2f a4 = Vector2f(a.m_Max.x, a.m_Min.y);
	if (math::IsOverlapping(Segment2f(a1, a2), b))
		return true;
	if (math::IsOverlapping(Segment2f(a1, a4), b))
		return true;
	if (math::IsOverlapping(Segment2f(a2, a3), b))
		return true;
	if (math::IsOverlapping(Segment2f(a4, a3), b))
		return true;

	return math::IsOverlapping(a, b.m_PointA) || math::IsOverlapping(a, b.m_PointB);
}

bool math::IsOverlapping(const Rect2f& a, const Triangle2f& b)
{
	return false;
}

bool math::IsOverlapping(const Rect2f& a, const Vector2f& b)
{
	return b.x >= a.m_Min.x && b.x <= a.m_Max.x
		&& b.y >= a.m_Min.y && b.y <= a.m_Max.y;
}

bool math::IsOverlapping(const Segment2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Segment2f& a, const Line2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Segment2f& a, const Ray2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Segment2f& a, const Rect2f& b)
{
	return math::IsOverlapping(b, a);
}

// https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
bool math::IsOverlapping(const Segment2f& a, const Segment2f& b)
{
	return ::AreCounterClockwiseOrdered(a.m_PointA, b.m_PointA, b.m_PointB) != ::AreCounterClockwiseOrdered(a.m_PointB, b.m_PointA, b.m_PointB)
		&& ::AreCounterClockwiseOrdered(a.m_PointA, a.m_PointB, b.m_PointA) != ::AreCounterClockwiseOrdered(a.m_PointA, a.m_PointB, b.m_PointB);
}

bool math::IsOverlapping(const Segment2f& a, const Triangle2f& b)
{
	if (math::IsOverlapping(a, Segment2f(b.m_PointA, b.m_PointB)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointB, b.m_PointC)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointC, b.m_PointA)))
		return true;

	// if either of the points are inside the triangle we are overlapping
	return math::IsOverlapping(a.m_PointA, b);
}

bool math::IsOverlapping(const Triangle2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Triangle2f& a, const Line2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Triangle2f& a, const Ray2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Triangle2f& a, const Rect2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Triangle2f& a, const Segment2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Triangle2f& a, const Triangle2f& b)
{
	return false;
}

// https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
bool math::IsOverlapping(const Triangle2f& a, const Vector2f& b)
{
	const float d0 = math::Sign(b, a.m_PointA, a.m_PointB);
	const float d1 = math::Sign(b, a.m_PointB, a.m_PointC);
	const float d2 = math::Sign(b, a.m_PointC, a.m_PointA);

	const bool has_neg = d0 < 0.f || d1 < 0.f || d2 < 0.f;
	const bool has_pos = d0 > 0.f || d1 > 0.f || d2 > 0.f;
	return !(has_neg && has_pos);
}

bool math::IsOverlapping(const Vector2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Vector2f& a, const Rect2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Vector2f& a, const Triangle2f& b)
{
	return math::IsOverlapping(b, a);
}

/*
bool math::IsOverlapping(const AABB& a, const AABB& b)
{

}

bool math::IsOverlapping(const AABB& a, const Line3f& b)
{

}

bool math::IsOverlapping(const AABB& a, const OBB& b)
{

}

bool math::IsOverlapping(const AABB& a, const Ray3f& b)
{

}

bool math::IsOverlapping(const AABB& a, const Segment3f& b)
{

}

bool math::IsOverlapping(const AABB& a, const Sphere3f& b)
{

}

bool math::IsOverlapping(const Line3f& a, const AABB& b)
{

}

bool math::IsOverlapping(const Line3f& a, const OBB& b)
{

}

bool math::IsOverlapping(const Line3f& a, const Ray3f& b)
{

}

bool math::IsOverlapping(const Line3f& a, const Segment3f& b)
{

}

bool math::IsOverlapping(const Line3f& a, const Sphere3f& b)
{

}

bool math::IsOverlapping(const OBB& a, const AABB& b)
{

}

bool math::IsOverlapping(const OBB& a, const Line3f& b)
{

}

bool math::IsOverlapping(const OBB& a, const OBB& b)
{

}

bool math::IsOverlapping(const OBB& a, const Ray3f& b)
{

}

bool math::IsOverlapping(const OBB& a, const Segment3f& b)
{

}

bool math::IsOverlapping(const OBB& a, const Sphere3f& b)
{

}

bool math::IsOverlapping(const Sphere3f& a, const Line3f& b)
{

}

bool math::IsOverlapping(const Sphere3f& a, const Ray3f& b)
{

}

bool math::IsOverlapping(const Sphere3f& a, const Segment3f& b)
{

}

bool math::IsOverlapping(const Sphere3f& a, const Sphere3f& b)
{

}

bool math::Raycast(const Ray2f& a, const Circle2f& b, Intersect2D& out_Result)
{

}

bool math::Raycast(const Ray2f& a, const Line2f& b, Intersect2D& out_Result)
{

}

bool math::Raycast(const Ray2f& a, const Ray2f& b, Intersect2D& out_Result)
{

}

bool math::Raycast(const Ray2f& a, const Segment2f& b, Intersect2D& out_Result)
{

}

bool math::Raycast(const Ray3f& a, const AABB& b, Intersect3D& out_Result)
{

}

bool math::Raycast(const Ray3f& a, const Plane& b, Intersect3D& out_Result)
{

}

bool math::Raycast(const Ray3f& a, const OBB& b, Intersect3D& out_Result)
{

}

bool math::Raycast(const Ray3f& a, const Sphere3f& b, Intersect3D& out_Result)
{

}

bool math::Sweep(const Circle2f& a, const Circle2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result)
{

}

bool math::Sweep(const Circle2f& a, const Line2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result)
{

}

bool math::Sweep(const Circle2f& a, const Ray2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result)
{

}

bool math::Sweep(const Circle2f& a, const Segment2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result)
{

}

bool math::Sweep(const Sphere3f& a, const AABB& b, const Vector3f& direction, const float distance, Intersect3D& out_Result)
{

}

bool math::Sweep(const Sphere3f& a, const Line3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result)
{

}

bool math::Sweep(const Sphere3f& a, const OBB& b, const Vector3f& direction, const float distance, Intersect3D& out_Result)
{

}

bool math::Sweep(const Sphere3f& a, const Ray3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result)
{

}

bool math::Sweep(const Sphere3f& a, const Segment3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result)
{

}

bool math::Sweep(const Sphere3f& a, const Sphere3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result)
{

}
*/