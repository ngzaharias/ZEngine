#pragma once

#include <Core/Vector.h>

class AABB;
class Circle2f;
class Line2f;
class Line3f;
class OBB;
class Plane;
class Ray2f;
class Ray3f;
class Rect2f;
class Segment2f;
class Segment3f;
class Sphere3f;

namespace math
{
	struct Intersect2D
	{
		/// \brief The contact point between the two objects.
		Vector2f m_HitPoint;
		/// \brief The normal of the face that was hit.
		Vector2f m_HitNormal;
		/// \brief Fraction along source and target where the hit happened.
		float m_HitFraction;
	};

	struct Intersect3D
	{
		/// \brief The contact point between the two objects.
		Vector3f m_HitPoint;
		/// \brief The normal of the face that was hit.
		Vector3f m_HitNormal;
		/// \brief Fraction along source and target where the hit happened.
		float m_HitFraction;
	};

	bool Intersection(const Line2f& a, const Circle2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line2f& a, const Line2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line2f& a, const Ray2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line2f& a, const Rect2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line2f& a, const Segment2f& b, Vector3f& out_IntersectPos);

	bool Intersection(const Ray2f& a, const Circle2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray2f& a, const Line2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray2f& a, const Ray2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray2f& a, const Rect2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray2f& a, const Segment2f& b, Vector3f& out_IntersectPos);

	bool Intersection(const Segment2f& a, const Circle2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment2f& a, const Line2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment2f& a, const Ray2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment2f& a, const Rect2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment2f& a, const Segment2f& b, Vector3f& out_IntersectPos);

	bool Intersection(const Line3f& line, const Plane& plane, Vector3f& out_IntersectPos);
	bool Intersection(const Ray3f& ray, const Plane& plane, Vector3f& out_IntersectPos);
	bool Intersection(const Segment3f& segment, const Plane& plane, Vector3f& out_IntersectPos);

	bool IsAxisOverlapping(const OBB& a, const OBB& b, const Vector3f& axis);

	//////////////////////////////////////////////////////////////////////////
	// 2D
	bool IsOverlapping(const Circle2f& a, const Circle2f& b);
	bool IsOverlapping(const Circle2f& a, const Line2f& b);
	bool IsOverlapping(const Circle2f& a, const Ray2f& b);
	bool IsOverlapping(const Circle2f& a, const Rect2f& b);
	bool IsOverlapping(const Circle2f& a, const Segment2f& b);
	bool IsOverlapping(const Circle2f& a, const Vector2f& b);

	bool IsOverlapping(const Line2f& a, const Circle2f& b);
	bool IsOverlapping(const Line2f& a, const Line2f& b);
	bool IsOverlapping(const Line2f& a, const Ray2f& b);
	bool IsOverlapping(const Line2f& a, const Rect2f& b);
	bool IsOverlapping(const Line2f& a, const Segment2f& b);

	bool IsOverlapping(const Ray2f& a, const Circle2f& b);
	bool IsOverlapping(const Ray2f& a, const Line2f& b);
	bool IsOverlapping(const Ray2f& a, const Ray2f& b);
	bool IsOverlapping(const Ray2f& a, const Rect2f& b);
	bool IsOverlapping(const Ray2f& a, const Segment2f& b);

	bool IsOverlapping(const Rect2f& a, const Circle2f& b);
	bool IsOverlapping(const Rect2f& a, const Line2f& b);
	bool IsOverlapping(const Rect2f& a, const Ray2f& b);
	bool IsOverlapping(const Rect2f& a, const Rect2f& b);
	bool IsOverlapping(const Rect2f& a, const Segment2f& b);
	bool IsOverlapping(const Rect2f& a, const Vector2f& b);

	bool IsOverlapping(const Segment2f& a, const Circle2f& b);
	bool IsOverlapping(const Segment2f& a, const Line2f& b);
	bool IsOverlapping(const Segment2f& a, const Ray2f& b);
	bool IsOverlapping(const Segment2f& a, const Rect2f& b);
	bool IsOverlapping(const Segment2f& a, const Segment2f& b);

	bool IsOverlapping(const Vector2f& a, const Circle2f& b);
	bool IsOverlapping(const Vector2f& a, const Rect2f& b);

	//////////////////////////////////////////////////////////////////////////
	// 3D
	bool IsOverlapping(const AABB& a, const AABB& b);
	bool IsOverlapping(const AABB& a, const Line3f& b);
	bool IsOverlapping(const AABB& a, const OBB& b);
	bool IsOverlapping(const AABB& a, const Ray3f& b);
	bool IsOverlapping(const AABB& a, const Segment3f& b);
	bool IsOverlapping(const AABB& a, const Sphere3f& b);

	bool IsOverlapping(const Line3f& a, const AABB& b);
	bool IsOverlapping(const Line3f& a, const OBB& b);
	bool IsOverlapping(const Line3f& a, const Ray3f& b);
	bool IsOverlapping(const Line3f& a, const Segment3f& b);
	bool IsOverlapping(const Line3f& a, const Sphere3f& b);

	bool IsOverlapping(const OBB& a, const AABB& b);
	bool IsOverlapping(const OBB& a, const Line3f& b);
	bool IsOverlapping(const OBB& a, const OBB& b);
	bool IsOverlapping(const OBB& a, const Ray3f& b);
	bool IsOverlapping(const OBB& a, const Segment3f& b);
	bool IsOverlapping(const OBB& a, const Sphere3f& b);

	bool IsOverlapping(const Sphere3f& a, const Line3f& b);
	bool IsOverlapping(const Sphere3f& a, const Ray3f& b);
	bool IsOverlapping(const Sphere3f& a, const Segment3f& b);
	bool IsOverlapping(const Sphere3f& a, const Sphere3f& b);

	//////////////////////////////////////////////////////////////////////////
	// 2D
	bool Raycast(const Ray2f& a, const Circle2f& b, Intersect2D& out_Result);
	bool Raycast(const Ray2f& a, const Line2f& b, Intersect2D& out_Result);
	bool Raycast(const Ray2f& a, const Ray2f& b, Intersect2D& out_Result);
	bool Raycast(const Ray2f& a, const Rect2f& b, Intersect2D& out_Result);
	bool Raycast(const Ray2f& a, const Segment2f& b, Intersect2D& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 3D
	bool Raycast(const Ray3f& a, const AABB& b, Intersect3D& out_Result);
	bool Raycast(const Ray3f& a, const Plane& b, Intersect3D& out_Result);
	bool Raycast(const Ray3f& a, const OBB& b, Intersect3D& out_Result);
	bool Raycast(const Ray3f& a, const Sphere3f& b, Intersect3D& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 2D
	bool Sweep(const Circle2f& a, const Circle2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Circle2f& a, const Line2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Circle2f& a, const Ray2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Circle2f& a, const Rect2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Circle2f& a, const Segment2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);

	bool Sweep(const Rect2f& a, const Circle2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Rect2f& a, const Line2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Rect2f& a, const Ray2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Rect2f& a, const Rect2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);
	bool Sweep(const Rect2f& a, const Segment2f& b, const Vector2f& direction, const float distance, Intersect2D& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 3D
	bool Sweep(const Sphere3f& a, const AABB& b, const Vector3f& direction, const float distance, Intersect3D& out_Result);
	bool Sweep(const Sphere3f& a, const Line3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result);
	bool Sweep(const Sphere3f& a, const OBB& b, const Vector3f& direction, const float distance, Intersect3D& out_Result);
	bool Sweep(const Sphere3f& a, const Ray3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result);
	bool Sweep(const Sphere3f& a, const Segment3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result);
	bool Sweep(const Sphere3f& a, const Sphere3f& b, const Vector3f& direction, const float distance, Intersect3D& out_Result);
}