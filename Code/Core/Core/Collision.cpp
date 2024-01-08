#include "Collision.h"

#include "Core/AABB.h"
#include "Core/Line.h"
#include "Core/OBB.h"
#include "Core/Plane.h"
#include "Core/Ray.h"
#include "Core/Sphere.h"
#include "Core/Vector.h"
#include "Core/VectorMath.h"

#include <algorithm>
#include <float.h>

bool math::AreCounterClockwiseOrderedXZ(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	return (c.z - a.z) * (b.x - a.x) > (b.z - a.z) * (c.x - a.x);
}

// https://github.com/BSVino/MathForGameDevelopers/blob/line-plane-intersection/math/collision.cpp
bool math::Intersection(const Line& line, const Plane& plane, Vector3f& out_IntersectPos)
{
	const Vector3f v = line.m_PointB - line.m_PointA;
	const Vector3f w = plane.m_Point - line.m_PointA;
	const float d0 = math::Dot(w, plane.m_Normal);
	const float d1 = math::Dot(v, plane.m_Normal);
	if (d1 == 0.f)
		return false;

	const float k = d0 / d1;
	out_IntersectPos = line.m_PointA + v * k;

	return k >= 0 && k <= 1;
}

// https://github.com/BSVino/MathForGameDevelopers/blob/line-plane-intersection/math/collision.cpp
bool math::Intersection(const Ray& ray, const Plane& plane, Vector3f& out_IntersectPos)
{
	Vector3f w = plane.m_Point - ray.m_OriginPos;
	const float d0 = math::Dot(w, plane.m_Normal);
	const float d1 = math::Dot(ray.m_Direction, plane.m_Normal);
	if (d1 == 0.f)
		return false;

	const float k = d0 / d1;
	out_IntersectPos = ray.m_OriginPos + ray.m_Direction * k;
	return true;
}

bool math::IntersectionXZ(const Sphere& sphere, const Vector3f& direction, const float distance, const Line& line, Intersect& out_Result)
{
	const Line sweepLine = Line(sphere.m_Position, sphere.m_Position + direction * distance);

	const Vector3f lineTagent = (line.m_PointB - line.m_PointA).Normalized();
	const Vector3f lineNormal = math::Cross(Vector3f::AxisY, lineTagent);

	// SOHCAHTOA - CAH
	// use the intersection angle of our ray and the line normal
	// use the smallest angle to account for both directions of a ray
	const float angle = Angle(direction, -lineNormal);
	const float theta = std::min(angle, 180.f - angle);
	const float adjacent = sphere.m_Radius;
	const float hypotenuse = adjacent / cos(theta);

	// increase the length of the line to account for the radius of the sphere
	const Vector3f edgeA = line.m_PointA - lineTagent * hypotenuse;
	const Vector3f edgeB = line.m_PointB + lineTagent * hypotenuse;

	Vector3f intersectPos;
	if (math::IntersectionXZ(sweepLine, line, intersectPos))
	{
		// project intersect position onto the original line to find the hit position
		const Vector3f hitPoint = math::ProjectOntoLineXZClamped(intersectPos, line.m_PointA, line.m_PointB);

		// project the hit position onto the circle line to find the position that the circle is touching the line
		const Vector3f touchPosition = math::ProjectOntoLineXZClamped(hitPoint, sweepLine.m_PointA, sweepLine.m_PointB);

		const float fraction = math::DistanceXZ(sweepLine.m_PointA, touchPosition) / distance;
		out_Result.m_HitPoint = hitPoint;
		out_Result.m_HitNormal = lineNormal;
		out_Result.m_HitFraction = fraction;
		return true;
	}
	return false;
}

bool math::IntersectionXZ(const Sphere& sphere, const Vector3f& direction, const float distance, const Ray& ray, Intersect& out_Result)
{
	const Ray circleRay = Ray(sphere.m_Position, direction);

	Vector3f intersectPos;
	if (math::IntersectionXZ(circleRay, ray, intersectPos))
	{
		// SOHCAHTOA - SOH
		// use the intersection angle of the two rays to find the distance to move backwards
		// use the smallest angle to account for both directions of a ray
		const float angle = Angle(ray.m_Direction, direction);
		const float theta = std::min(angle, 180.f - angle);
		const float opposite = sphere.m_Radius;
		const float hypotenuse = opposite / sin(theta);
		intersectPos -= direction * hypotenuse;

		const float fraction = math::DistanceXZ(sphere.m_Position, intersectPos) / distance;
		const Vector3f normal = math::Cross(Vector3f::AxisY, ray.m_Direction);

		out_Result.m_HitPoint = intersectPos;
		out_Result.m_HitNormal = normal;
		out_Result.m_HitFraction = fraction;
		return true;
	}
	return false;
}

bool math::IntersectionXZ(const Line& a, const Line& b, Vector3f& out_IntersectPos)
{
	if (!IsOverlappingXZ(a, b))
		return false;

	const float det = (a.m_PointA.x - a.m_PointB.x) * (b.m_PointA.z - b.m_PointB.z) - (a.m_PointA.z - a.m_PointB.z) * (b.m_PointA.x - b.m_PointB.x);
	if (det == 0.f)
		return false;

	out_IntersectPos.x = ((b.m_PointA.x - b.m_PointB.x) * (a.m_PointA.x * a.m_PointB.z - a.m_PointA.z * a.m_PointB.x) - (a.m_PointA.x - a.m_PointB.x) * (b.m_PointA.x * b.m_PointB.z - b.m_PointA.z * b.m_PointB.x)) / det;
	out_IntersectPos.z = ((b.m_PointA.z - b.m_PointB.z) * (a.m_PointA.x * a.m_PointB.z - a.m_PointA.z * a.m_PointB.x) - (a.m_PointA.z - a.m_PointB.z) * (b.m_PointA.x * b.m_PointB.z - b.m_PointA.z * b.m_PointB.x)) / det;
	return true;
}

// https://www.anycodings.com/1questions/1479872/determining-if-two-rays-intersect
bool math::IntersectionXZ(const Ray& a, const Ray& b, Vector3f& out_IntersectPos)
{
	const float det = (b.m_Direction.x * a.m_Direction.z) - (b.m_Direction.z * a.m_Direction.x);
	if (det == 0.f)
		return false;

	const float dx = b.m_OriginPos.x - a.m_OriginPos.x;
	const float dz = b.m_OriginPos.z - a.m_OriginPos.z;
	const float u = (dz * b.m_Direction.x - dx * b.m_Direction.z) / det;
	const float v = (dz * a.m_Direction.x - dx * a.m_Direction.z) / det;
	if (u < 0.f || v < 0.f)
		return false;

	out_IntersectPos.x = a.m_OriginPos.x + a.m_Direction.x * u;
	out_IntersectPos.z = a.m_OriginPos.z + a.m_Direction.z * u;
	return true;
}

// https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat
bool math::IsAxisOverlapping(const OBB& a, const OBB& b, const Vector3f& axis)
{
	if (axis == Vector3f::Zero)
		return true;

	float aDis, bDis;
	float aMin = FLT_MAX;
	float aMax = -FLT_MAX;
	float bMin = FLT_MAX;
	float bMax = -FLT_MAX;
	for (int32 i = 0; i < 8; ++i)
	{
		aDis = math::Dot(a.m_Points[i], axis);
		aMin = (aDis < aMin) ? aDis : aMin;
		aMax = (aDis > aMax) ? aDis : aMax;
		bDis = math::Dot(b.m_Points[i], axis);
		bMin = (bDis < bMin) ? bDis : bMin;
		bMax = (bDis > bMax) ? bDis : bMax;
	}

	return ((aMax >= bMin) && (bMax >= aMin));
}

bool math::IsOverlapping(const AABB& a, const AABB& b)
{
	return (a.m_Min.x <= b.m_Max.x && a.m_Max.x > b.m_Min.x)
		&& (a.m_Min.y <= b.m_Max.y && a.m_Max.y > b.m_Min.y)
		&& (a.m_Min.z <= b.m_Max.z && a.m_Max.z > b.m_Min.z);
}

// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool math::IsOverlapping(const AABB& aabb, const Ray& ray)
{
	const Vector3f reciprocal = Vector3f(1.f / ray.m_Direction.x, 1.f / ray.m_Direction.y, 1.f / ray.m_Direction.z);

	const float t1 = (aabb.m_Min.x - ray.m_OriginPos.x) * reciprocal.x;
	const float t2 = (aabb.m_Max.x - ray.m_OriginPos.x) * reciprocal.x;
	const float t3 = (aabb.m_Min.y - ray.m_OriginPos.y) * reciprocal.y;
	const float t4 = (aabb.m_Max.y - ray.m_OriginPos.y) * reciprocal.y;
	const float t5 = (aabb.m_Min.z - ray.m_OriginPos.z) * reciprocal.z;
	const float t6 = (aabb.m_Max.z - ray.m_OriginPos.z) * reciprocal.z;

	const float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	const float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax < 0.f)
		return false;
	if (tmin > tmax)
		return false;
	return true;
}

// https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat
bool math::IsOverlapping(const OBB& a, const OBB& b)
{
	// "up"
	const Vector3f a0(a.m_Points[4] - a.m_Points[0]);
	if (!IsAxisOverlapping(a, b, a0)) return false;

	// "right"
	const Vector3f a1(a.m_Points[3] - a.m_Points[0]);
	if (!IsAxisOverlapping(a, b, a1)) return false;

	// "forward"
	const Vector3f a2(a.m_Points[1] - a.m_Points[0]);
	if (!IsAxisOverlapping(a, b, a2)) return false;

	// "up"
	const Vector3f b0 = (b.m_Points[4] - b.m_Points[0]);
	if (!IsAxisOverlapping(a, b, b0)) return false;

	// "right"
	const Vector3f b1 = (b.m_Points[3] - b.m_Points[0]);
	if (!IsAxisOverlapping(a, b, b1)) return false;

	// "forward"
	const Vector3f b2 = (b.m_Points[1] - b.m_Points[0]);
	if (!IsAxisOverlapping(a, b, b2)) return false;

	// cross products
	if (!IsAxisOverlapping(a, b, math::Cross(a0, b0))) return false;
	if (!IsAxisOverlapping(a, b, math::Cross(a0, b1))) return false;
	if (!IsAxisOverlapping(a, b, math::Cross(a0, b2))) return false;

	if (!IsAxisOverlapping(a, b, math::Cross(a1, b0))) return false;
	if (!IsAxisOverlapping(a, b, math::Cross(a1, b1))) return false;
	if (!IsAxisOverlapping(a, b, math::Cross(a1, b2))) return false;

	if (!IsAxisOverlapping(a, b, math::Cross(a2, b0))) return false;
	if (!IsAxisOverlapping(a, b, math::Cross(a2, b1))) return false;
	if (!IsAxisOverlapping(a, b, math::Cross(a2, b2))) return false;

	return true;
}

bool math::IsOverlappingXZ(const Sphere& a, const Sphere& b)
{
	const float distanceSqr = math::DistanceXZSqr(a.m_Position, b.m_Position);
	const float radiiSqr = math::Sqr(a.m_Radius + b.m_Radius);
	return distanceSqr < radiiSqr;
}

bool math::IsOverlappingXZ(const Sphere& sphere, const Line& line)
{
	const Vector3f intersectPos = math::ProjectOntoLineXZClamped(sphere.m_Position, line.m_PointA, line.m_PointB);
	const float intersectSqr = math::DistanceXZSqr(sphere.m_Position, intersectPos);
	return intersectSqr <= math::Sqr(sphere.m_Radius);
}

bool math::IsOverlappingXZ(const Sphere& sphere, const Ray& ray)
{
	const Vector3f intersectPos = math::ProjectOntoRayXZ(sphere.m_Position, ray.m_OriginPos, ray.m_Direction);
	const float intersectSqr = math::DistanceXZSqr(sphere.m_Position, intersectPos);
	return intersectSqr <= math::Sqr(sphere.m_Radius);
}

// https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
bool math::IsOverlappingXZ(const Line& a, const Line& b)
{
	return AreCounterClockwiseOrderedXZ(a.m_PointA, b.m_PointA, b.m_PointB) != AreCounterClockwiseOrderedXZ(a.m_PointB, b.m_PointA, b.m_PointB)
		&& AreCounterClockwiseOrderedXZ(a.m_PointA, a.m_PointB, b.m_PointA) != AreCounterClockwiseOrderedXZ(a.m_PointA, a.m_PointB, b.m_PointB);
}

bool math::IsOverlappingXZ(const Ray& a, const Ray& b)
{
	const float det = (b.m_Direction.x * a.m_Direction.z) - (b.m_Direction.z * a.m_Direction.x);
	if (det == 0.f)
		return false;

	const float dx = b.m_OriginPos.x - a.m_OriginPos.x;
	const float dz = b.m_OriginPos.z - a.m_OriginPos.z;
	const float u = (dz * b.m_Direction.x - dx * b.m_Direction.z) / det;
	const float v = (dz * a.m_Direction.x - dx * a.m_Direction.z) / det;
	return u >= 0.f && v >= 0.f;
}
