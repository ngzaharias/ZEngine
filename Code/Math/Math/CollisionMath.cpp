#include "CollisionMath.h"

#include "Math/AABB.h"
#include "Math/Circle.h"
#include "Math/Line.h"
#include "Math/OBB.h"
#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Math/Segment.h"
#include "Math/Sphere.h"
#include "Math/Triangle.h"
#include "Math/Vector.h"
#include "Math/VectorMath.h"

#include <algorithm>
#include <array>
#include <float.h>

namespace
{
	using Array3 = Vector2f[3];
	using Array4 = Vector2f[4];

	bool AreCounterClockwiseOrdered(const Vector2f& a, const Vector2f& b, const Vector2f& c)
	{
		return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
	}

	Vector2f GetInterval(const Array3& points, const Vector2f& axis)
	{
		const float d1 = math::Dot(axis, points[0]);
		const float d2 = math::Dot(axis, points[1]);
		const float d3 = math::Dot(axis, points[2]);

		const float min = std::min({ d1, d2, d3 });
		const float max = std::max({ d1, d2, d3 });

		return Vector2f(min, max);
	}

	Vector2f GetInterval(const Array4& points, const Vector2f& axis)
	{
		const float d1 = math::Dot(axis, points[0]);
		const float d2 = math::Dot(axis, points[1]);
		const float d3 = math::Dot(axis, points[2]);
		const float d4 = math::Dot(axis, points[3]);

		const float min = std::min({ d1, d2, d3, d4 });
		const float max = std::max({ d1, d2, d3, d4 });

		return Vector2f(min, max);
	}

	bool IsAxisSeparated(const Array3& a, const Array3& b, const Vector2f& axis)
	{
		const Vector2f i1 = GetInterval(a, axis);
		const Vector2f i2 = GetInterval(b, axis);
		return i1.x > i2.y || i2.x > i1.y;
	}

	bool IsAxisSeparated(const Array4& a, const Array3& b, const Vector2f& axis)
	{
		const Vector2f i1 = GetInterval(a, axis);
		const Vector2f i2 = GetInterval(b, axis);
		return i1.x > i2.y || i2.x > i1.y;
	}
}

bool math::Intersection(const Line3f& a, const Plane3f& b, Vector3f& out_IntersectPos)
{
	const Vector3f v = a.m_PointB - a.m_PointA;
	const float d0 = math::Dot(v, b.m_Normal);
	if (d0 == 0.f)
		return false;

	const Vector3f w = a.m_PointA - b.m_Point;
	const float d1 = math::Dot(w, b.m_Normal);
	const float d2 = d1 / d0;
	out_IntersectPos = a.m_PointA - v * d2;
	return true;
}

// https://github.com/BSVino/MathForGameDevelopers/blob/line-plane-intersection/math/collision.cpp
bool math::Intersection(const Ray3f& a, const Plane3f& b, Vector3f& out_IntersectPos)
{
	const float d0 = math::Dot(a.m_Direction, b.m_Normal);
	if (d0 == 0.f)
		return false;

	const Vector3f w = b.m_Point - a.m_Position;
	const float d1 = math::Dot(w, b.m_Normal);
	const float d2 = d1 / d0;
	out_IntersectPos = a.m_Position + a.m_Direction * d2;
	return d2 >= 0.f;
}

// https://github.com/BSVino/MathForGameDevelopers/blob/line-plane-intersection/math/collision.cpp
bool math::Intersection(const Segment3f& a, const Plane3f& b, Vector3f& out_IntersectPos)
{
	const Vector3f v = a.m_PointB - a.m_PointA;
	const float d0 = math::Dot(v, b.m_Normal);
	if (d0 == 0.f)
		return false;

	const Vector3f w = b.m_Point - a.m_PointA;
	const float d1 = math::Dot(w, b.m_Normal);
	const float d2 = d1 / d0;
	out_IntersectPos = a.m_PointA + v * d2;
	return d2 >= 0.f && d2 <= 1.f;
}

bool math::IsOverlapping(const AABB2f& a, const AABB2f& b)
{
	return (a.m_Min.x <= b.m_Max.x && a.m_Max.x >= b.m_Min.x)
		&& (a.m_Min.y <= b.m_Max.y && a.m_Max.y >= b.m_Min.y);
}

bool math::IsOverlapping(const AABB2f& a, const Circle2f& b)
{
	const Vector2f half = (a.m_Max - a.m_Min) * 0.5f;
	const Vector2f centre = a.m_Min + half;
	const Vector2f dist = math::Abs(b.m_Position - centre);
	if (dist.x > (half.x + b.m_Radius))
		return false;
	if (dist.y > (half.y + b.m_Radius))
		return false;

	if (dist.x <= half.x)
		return true;
	if (dist.y <= half.y)
		return true;

	const float dSqr = math::Sqr(dist.x - half.x) + math::Sqr(dist.y - half.y);
	return dSqr <= math::Sqr(b.m_Radius);
}

bool math::IsOverlapping(const AABB2f& a, const Line2f& b)
{
	if (math::IsOverlapping(b, Segment2f(a.m_Min, Vector2f(a.m_Min.x, a.m_Max.y))))
		return true;
	if (math::IsOverlapping(b, Segment2f(a.m_Min, Vector2f(a.m_Max.x, a.m_Min.y))))
		return true;
	if (math::IsOverlapping(b, Segment2f(Vector2f(a.m_Min.x, a.m_Max.y), a.m_Max)))
		return true;
	if (math::IsOverlapping(b, Segment2f(Vector2f(a.m_Max.x, a.m_Min.y), a.m_Max)))
		return true;
	return false;
}

bool math::IsOverlapping(const AABB2f& a, const Ray2f& b)
{
	const Vector2f reciprocal = Vector2f(
		1.f / b.m_Direction.x,
		1.f / b.m_Direction.y);

	const float t1 = (a.m_Min.x - b.m_Position.x) * reciprocal.x;
	const float t2 = (a.m_Max.x - b.m_Position.x) * reciprocal.x;
	const float t3 = (a.m_Min.y - b.m_Position.y) * reciprocal.y;
	const float t4 = (a.m_Max.y - b.m_Position.y) * reciprocal.y;

	const float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
	const float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

	if (tmax < 0.f)
		return false;
	if (tmin > tmax)
		return false;
	return true;
}

bool math::IsOverlapping(const AABB2f& a, const Segment2f& b)
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

bool math::IsOverlapping(const AABB2f& a, const Triangle2f& b)
{
	const Array4 p1 = { a.m_Min, Vector2f(a.m_Min.x, a.m_Max.y), a.m_Max, Vector2f(a.m_Max.x, a.m_Min.y) };
	const Array3 p2 = { b.m_PointA, b.m_PointB, b.m_PointC };

	// Rect
	for (int32 i = 0; i < 4; ++i)
	{
		const int32 j = (i + 1) % 4;
		const Vector2f tangent = p1[i] - p1[j];
		const Vector2f normal = math::Perpendicular(tangent);
		if (IsAxisSeparated(p1, p2, normal))
			return false;
	}

	// Triangle
	for (int32 i = 0; i < 3; ++i)
	{
		const int32 j = (i + 1) % 3;
		const Vector2f tangent = p2[i] - p2[j];
		const Vector2f normal = math::Perpendicular(tangent);
		if (IsAxisSeparated(p1, p2, normal))
			return false;
	}

	return true;
}

bool math::IsOverlapping(const AABB2f& a, const Vector2f& b)
{
	return b.x >= a.m_Min.x && b.x <= a.m_Max.x
		&& b.y >= a.m_Min.y && b.y <= a.m_Max.y;
}

// https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
bool math::IsOverlapping(const Circle2f& a, const AABB2f& b)
{
	return math::IsOverlapping(b, a);
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

bool math::IsOverlapping(const Circle2f& a, const Segment2f& b)
{
	const Vector2f intersectPos = math::Project(a.m_Position, b);
	const float intersectSqr = math::DistanceSqr(a.m_Position, intersectPos);
	return intersectSqr <= math::Sqr(a.m_Radius);
}

bool math::IsOverlapping(const Circle2f& a, const Triangle2f& b)
{
	if (math::IsOverlapping(a.m_Position, b))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointA, b.m_PointB)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointB, b.m_PointC)))
		return true;
	if (math::IsOverlapping(a, Segment2f(b.m_PointC, b.m_PointA)))
		return true;
	return false;
}

bool math::IsOverlapping(const Circle2f& a, const Vector2f& b)
{
	const float distanceSqr = math::DistanceSqr(a.m_Position, b);
	const float radiiSqr = math::Sqr(a.m_Radius);
	return distanceSqr <= radiiSqr;
}

// https://stackoverflow.com/questions/99353/how-to-test-if-a-line-segment-intersects-an-axis-aligned-rectange-in-2d
// https://stackoverflow.com/questions/3746274/line-intersection-with-aabb-rectangle
bool math::IsOverlapping(const Line2f& a, const AABB2f& b)
{
	return math::IsOverlapping(b, a);
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
	const float det = math::Dot(v1, v2);
	if (det == 0.f)
		return false;

	const Vector2f w = b.m_Position - a.m_PointA;
	const float t1 = math::Cross(v1, w) / det;
	return t1 >= 0.f;
}

// https://stackoverflow.com/questions/14307158/how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
bool math::IsOverlapping(const Line2f& a, const Segment2f& b)
{
	const Vector2f v1 = b.m_PointB - b.m_PointA;
	const Vector2f v2 = math::Perpendicular(a.m_PointB - a.m_PointA);
	const float det = math::Dot(v1, v2);
	if (det == 0.f)
		return false;

	const Vector2f w = a.m_PointA - b.m_PointA;
	const float t = math::Dot(w, v2) / det;
	return t >= 0.f && t <= 1.f;
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

bool math::IsOverlapping(const Ray2f& a, const AABB2f& b)
{
	return math::IsOverlapping(b, a);
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

// https://stackoverflow.com/questions/14307158/how-do-you-check-for-intersection-between-a-line-segment-and-a-line-ray-emanatin
bool math::IsOverlapping(const Ray2f& a, const Segment2f& b)
{
	const Vector2f v1 = b.m_PointB - b.m_PointA;
	const Vector2f v2 = math::Perpendicular(a.m_Direction);
	const float det = math::Dot(v1, v2);
	if (det == 0.f)
		return false;

	const Vector2f w = a.m_Position - b.m_PointA;
	const float t1 = math::Cross(v1, w) / det;
	const float t2 = math::Dot(w, v2) / det;
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

bool math::IsOverlapping(const Segment2f& a, const AABB2f& b)
{
	return math::IsOverlapping(b, a);
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

// https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
bool math::IsOverlapping(const Segment2f& a, const Segment2f& b)
{
	return AreCounterClockwiseOrdered(a.m_PointA, b.m_PointA, b.m_PointB) != AreCounterClockwiseOrdered(a.m_PointB, b.m_PointA, b.m_PointB)
		&& AreCounterClockwiseOrdered(a.m_PointA, a.m_PointB, b.m_PointA) != AreCounterClockwiseOrdered(a.m_PointA, a.m_PointB, b.m_PointB);
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

bool math::IsOverlapping(const Triangle2f& a, const AABB2f& b)
{
	return math::IsOverlapping(b, a);
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

bool math::IsOverlapping(const Triangle2f& a, const Segment2f& b)
{
	return math::IsOverlapping(b, a);
}

// https://code.tutsplus.com/collision-detection-using-the-separating-axis-theorem--gamedev-169t
bool math::IsOverlapping(const Triangle2f& a, const Triangle2f& b)
{
	const Array3 p1 = { a.m_PointA, a.m_PointB, a.m_PointC };
	const Array3 p2 = { b.m_PointA, b.m_PointB, b.m_PointC };

	// Triangle A
	for (int32 i = 0; i < 3; ++i)
	{
		const int32 j = (i + 1) % 3;
		const Vector2f tangent = p1[i] - p1[j];
		const Vector2f normal = math::Perpendicular(tangent);
		if (IsAxisSeparated(p1, p2, normal))
			return false;
	}

	// Triangle B
	for (int32 i = 0; i < 3; ++i)
	{
		const int32 j = (i + 1) % 3;
		const Vector2f tangent = p2[i] - p2[j];
		const Vector2f normal = math::Perpendicular(tangent);
		if (IsAxisSeparated(p1, p2, normal))
			return false;
	}

	return true;
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

bool math::IsOverlapping(const Vector2f& a, const AABB2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Vector2f& a, const Circle2f& b)
{
	return math::IsOverlapping(b, a);
}

bool math::IsOverlapping(const Vector2f& a, const Triangle2f& b)
{
	return math::IsOverlapping(b, a);
}