#pragma once

#include "Math/Vector.h"

class AABB2f;
class AABB2i;
class AABB3f;
class Circle2f;
class Line2f;
class Line3f;
class OBB2f;
class OBB3f;
class Plane3f;
class Ray2f;
class Ray3f;
class Segment2f;
class Segment3f;
class Sphere3f;
class Triangle2f;
class Triangle3f;

namespace math
{
	struct Intersect2f
	{
		/// \brief The contact point between the two objects.
		Vector2f m_HitPoint;
		/// \brief The normal of the face that was hit.
		Vector2f m_HitNormal;
		/// \brief Fraction along source and target where the hit happened.
		float m_HitFraction;
	};

	struct Intersect3f
	{
		/// \brief The contact point between the two objects.
		Vector3f m_HitPoint;
		/// \brief The normal of the face that was hit.
		Vector3f m_HitNormal;
		/// \brief Fraction along source and target where the hit happened.
		float m_HitFraction;
	};

	//////////////////////////////////////////////////////////////////////////
	// 2D

	bool Intersection(const Line2f& a, const Line2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line2f& a, const Ray2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line2f& a, const Segment2f& b, Vector3f& out_IntersectPos);

	bool Intersection(const Ray2f& a, const Line2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray2f& a, const Ray2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray2f& a, const Segment2f& b, Vector3f& out_IntersectPos);

	bool Intersection(const Segment2f& a, const Line2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment2f& a, const Ray2f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment2f& a, const Segment2f& b, Vector3f& out_IntersectPos);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool Intersection(const AABB3f& a, const Segment3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Line3f& a, const Plane3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray3f& a, const AABB3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray3f& a, const Plane3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray3f& a, const Sphere3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment3f& a, const Plane3f& b, Vector3f& out_IntersectPos);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	/// \brief Returns true if AABB A is overlapping AABB B.
	bool IsOverlapping(const AABB2f& a, const AABB2f& b);
	/// \brief Returns true if AABB A is overlapping AABB B.
	bool IsOverlapping(const AABB2i& a, const AABB2i& b);
	/// \brief Returns true if a AABB is overlapping a Circle.
	bool IsOverlapping(const AABB2f& a, const Circle2f& b);
	/// \brief Returns true if a AABB is overlapping a Line.
	bool IsOverlapping(const AABB2f& a, const Line2f& b);
	/// \brief Returns true if a AABB is overlapping an OBB.
	bool IsOverlapping(const AABB2f& a, const OBB2f& b);
	/// \brief Returns true if a AABB is overlapping a Ray.
	bool IsOverlapping(const AABB2f& a, const Ray2f& b);
	/// \brief Returns true if a AABB is overlapping a Segment.
	bool IsOverlapping(const AABB2f& a, const Segment2f& b);
	/// \brief Returns true if a AABB is overlapping a Triangle.
	bool IsOverlapping(const AABB2f& a, const Triangle2f& b);
	/// \brief Returns true if a AABB is overlapping a Point.
	bool IsOverlapping(const AABB2f& a, const Vector2f& b);
	bool IsOverlapping(const AABB2i& a, const Vector2i& b);

	/// \brief Returns true if a Circle is overlapping a AABB.
	bool IsOverlapping(const Circle2f& a, const AABB2f& b);
	/// \brief Returns true if Circle A is overlapping Circle B.
	bool IsOverlapping(const Circle2f& a, const Circle2f& b);
	/// \brief Returns true if a Circle is overlapping a Line.
	bool IsOverlapping(const Circle2f& a, const Line2f& b);
	/// \brief Returns true if a Circle is overlapping an OBB.
	bool IsOverlapping(const Circle2f& a, const OBB2f& b);
	/// \brief Returns true if a Circle is overlapping a Ray.
	bool IsOverlapping(const Circle2f& a, const Ray2f& b);
	/// \brief Returns true if a Circle is overlapping a Segment.
	bool IsOverlapping(const Circle2f& a, const Segment2f& b);
	/// \brief Returns true if a Circle is overlapping a Triangle.
	bool IsOverlapping(const Circle2f& a, const Triangle2f& b);
	/// \brief Returns true if a Circle is overlapping a Point.
	bool IsOverlapping(const Circle2f& a, const Vector2f& b);

	/// \brief Returns true if a Line is overlapping a AABB.
	bool IsOverlapping(const Line2f& a, const AABB2f& b);
	/// \brief Returns true if a Line is overlapping a Circle.
	bool IsOverlapping(const Line2f& a, const Circle2f& b);
	/// \brief Returns true if Line A is overlapping Line B.
	bool IsOverlapping(const Line2f& a, const Line2f& b);
	/// \brief Returns true if a Line is overlapping an OBB.
	bool IsOverlapping(const Line2f& a, const OBB2f& b);
	/// \brief Returns true if a Line is overlapping a Ray.
	bool IsOverlapping(const Line2f& a, const Ray2f& b);
	/// \brief Returns true if a Line is overlapping a Segment.
	bool IsOverlapping(const Line2f& a, const Segment2f& b);
	/// \brief Returns true if a Line is overlapping a Triangle.
	bool IsOverlapping(const Line2f& a, const Triangle2f& b);

	/// \brief Returns true if a OBB is overlapping a AABB.
	bool IsOverlapping(const OBB2f& a, const AABB2f& b);
	/// \brief Returns true if a OBB is overlapping a Circle.
	bool IsOverlapping(const OBB2f& a, const Circle2f& b);
	/// \brief Returns true if a OBB is overlapping a Line.
	bool IsOverlapping(const OBB2f& a, const Line2f& b);
	/// \brief Returns true if OBB A is overlapping OBB B.
	bool IsOverlapping(const OBB2f& a, const OBB2f& b);
	/// \brief Returns true if a OBB is overlapping a Ray.
	bool IsOverlapping(const OBB2f& a, const Ray2f& b);
	/// \brief Returns true if a OBB is overlapping a Segment.
	bool IsOverlapping(const OBB2f& a, const Segment2f& b);
	/// \brief Returns true if a OBB is overlapping a Triangle.
	bool IsOverlapping(const OBB2f& a, const Triangle2f& b);

	/// \brief Returns true if a Ray is overlapping a AABB.
	bool IsOverlapping(const Ray2f& a, const AABB2f& b);
	/// \brief Returns true if a Ray is overlapping a Circle.
	bool IsOverlapping(const Ray2f& a, const Circle2f& b);
	/// \brief Returns true if a Ray is overlapping a Line.
	bool IsOverlapping(const Ray2f& a, const Line2f& b);
	/// \brief Returns true if a Ray is overlapping an OBB.
	bool IsOverlapping(const Ray2f& a, const OBB2f& b);
	/// \brief Returns true if Ray A is overlapping Ray B.
	bool IsOverlapping(const Ray2f& a, const Ray2f& b);
	/// \brief Returns true if a Ray is overlapping a Segment.
	bool IsOverlapping(const Ray2f& a, const Segment2f& b);
	/// \brief Returns true if a Ray is overlapping a Triangle.
	bool IsOverlapping(const Ray2f& a, const Triangle2f& b);

	/// \brief Returns true if a Segment is overlapping a AABB.
	bool IsOverlapping(const Segment2f& a, const AABB2f& b);
	/// \brief Returns true if a Segment is overlapping a Circle.
	bool IsOverlapping(const Segment2f& a, const Circle2f& b);
	/// \brief Returns true if a Segment is overlapping a Line.
	bool IsOverlapping(const Segment2f& a, const Line2f& b);
	/// \brief Returns true if a Segment is overlapping an OBB.
	bool IsOverlapping(const Segment2f& a, const OBB2f& b);
	/// \brief Returns true if a Segment is overlapping a Ray.
	bool IsOverlapping(const Segment2f& a, const Ray2f& b);
	/// \brief Returns true if Segment A is overlapping Segment B.
	bool IsOverlapping(const Segment2f& a, const Segment2f& b);
	/// \brief Returns true if a Segment is overlapping a Triangle.
	bool IsOverlapping(const Segment2f& a, const Triangle2f& b);

	/// \brief Returns true if a Triangle is overlapping a AABB.
	bool IsOverlapping(const Triangle2f& a, const AABB2f& b);
	/// \brief Returns true if a Triangle is overlapping a Circle.
	bool IsOverlapping(const Triangle2f& a, const Circle2f& b);
	/// \brief Returns true if a Triangle is overlapping a Line.
	bool IsOverlapping(const Triangle2f& a, const Line2f& b);
	/// \brief Returns true if a Triangle is overlapping an OBB.
	bool IsOverlapping(const Triangle2f& a, const OBB2f& b);
	/// \brief Returns true if a Triangle is overlapping a Ray.
	bool IsOverlapping(const Triangle2f& a, const Ray2f& b);
	/// \brief Returns true if a Triangle is overlapping a Segment.
	bool IsOverlapping(const Triangle2f& a, const Segment2f& b);
	/// \brief Returns true if Triangle A is overlapping Triangle B.
	bool IsOverlapping(const Triangle2f& a, const Triangle2f& b);
	/// \brief Returns true if a Triangle is overlapping a Point.
	bool IsOverlapping(const Triangle2f& a, const Vector2f& b);

	/// \brief Returns true if a Point is overlapping a AABB.
	bool IsOverlapping(const Vector2f& a, const AABB2f& b);
	/// \brief Returns true if a Point is overlapping a Circle.
	bool IsOverlapping(const Vector2f& a, const Circle2f& b);
	/// \brief Returns true if a Point is overlapping an OBB.
	bool IsOverlapping(const Vector2f& a, const OBB2f& b);
	/// \brief Returns true if a Point is overlapping a Triangle.
	bool IsOverlapping(const Vector2f& a, const Triangle2f& b);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool IsOverlapping(const AABB3f& a, const AABB3f& b);
	bool IsOverlapping(const AABB3f& a, const Line3f& b);
	bool IsOverlapping(const AABB3f& a, const OBB3f& b);
	bool IsOverlapping(const AABB3f& a, const Plane3f& b);
	bool IsOverlapping(const AABB3f& a, const Ray3f& b);
	bool IsOverlapping(const AABB3f& a, const Segment3f& b);
	bool IsOverlapping(const AABB3f& a, const Sphere3f& b);

	bool IsOverlapping(const Line3f& a, const AABB3f& b);
	bool IsOverlapping(const Line3f& a, const OBB3f& b);
	bool IsOverlapping(const Line3f& a, const Plane3f& b);
	bool IsOverlapping(const Line3f& a, const Ray3f& b);
	bool IsOverlapping(const Line3f& a, const Segment3f& b);
	bool IsOverlapping(const Line3f& a, const Sphere3f& b);

	bool IsOverlapping(const OBB3f& a, const AABB3f& b);
	bool IsOverlapping(const OBB3f& a, const Line3f& b);
	bool IsOverlapping(const OBB3f& a, const OBB3f& b);
	bool IsOverlapping(const OBB3f& a, const Plane3f& b);
	bool IsOverlapping(const OBB3f& a, const Ray3f& b);
	bool IsOverlapping(const OBB3f& a, const Segment3f& b);
	bool IsOverlapping(const OBB3f& a, const Sphere3f& b);

	bool IsOverlapping(const Plane3f& a, const AABB3f& b);
	bool IsOverlapping(const Plane3f& a, const Line3f& b);
	bool IsOverlapping(const Plane3f& a, const OBB3f& b);
	bool IsOverlapping(const Plane3f& a, const Plane3f& b);
	bool IsOverlapping(const Plane3f& a, const Ray3f& b);
	bool IsOverlapping(const Plane3f& a, const Segment3f& b);
	bool IsOverlapping(const Plane3f& a, const Sphere3f& b);

	bool IsOverlapping(const Ray3f& a, const AABB3f& b);
	bool IsOverlapping(const Ray3f& a, const Line3f& b);
	bool IsOverlapping(const Ray3f& a, const OBB3f& b);
	bool IsOverlapping(const Ray3f& a, const Plane3f& b);
	bool IsOverlapping(const Ray3f& a, const Ray3f& b);
	bool IsOverlapping(const Ray3f& a, const Segment3f& b);
	bool IsOverlapping(const Ray3f& a, const Sphere3f& b);

	bool IsOverlapping(const Segment3f& a, const AABB3f& b);
	bool IsOverlapping(const Segment3f& a, const Line3f& b);
	bool IsOverlapping(const Segment3f& a, const OBB3f& b);
	bool IsOverlapping(const Segment3f& a, const Plane3f& b);
	bool IsOverlapping(const Segment3f& a, const Ray3f& b);
	bool IsOverlapping(const Segment3f& a, const Segment3f& b);
	bool IsOverlapping(const Segment3f& a, const Sphere3f& b);

	bool IsOverlapping(const Sphere3f& a, const AABB3f& b);
	bool IsOverlapping(const Sphere3f& a, const Line3f& b);
	bool IsOverlapping(const Sphere3f& a, const OBB3f& b);
	bool IsOverlapping(const Sphere3f& a, const Plane3f& b);
	bool IsOverlapping(const Sphere3f& a, const Ray3f& b);
	bool IsOverlapping(const Sphere3f& a, const Segment3f& b);
	bool IsOverlapping(const Sphere3f& a, const Sphere3f& b);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	AABB2f OverlappingArea(const AABB2f& a, const AABB2f& b);
	AABB2i OverlappingArea(const AABB2i& a, const AABB2i& b);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	bool Raycast(const Ray2f& ray, const float distance, const Circle2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Line2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Ray2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const AABB2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Segment2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Triangle2f& target, Intersect2f& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool Raycast(const Ray3f& ray, const float distance, const AABB3f& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const Plane3f& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const OBB3f& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const Sphere3f& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const Triangle3f& target, Intersect3f& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	bool Sweep(const AABB2f& rect, const Vector2f& direction, const float distance, const Circle2f& target, Intersect2f& out_Result);
	bool Sweep(const AABB2f& rect, const Vector2f& direction, const float distance, const Line2f& target, Intersect2f& out_Result);
	bool Sweep(const AABB2f& rect, const Vector2f& direction, const float distance, const Ray2f& target, Intersect2f& out_Result);
	bool Sweep(const AABB2f& rect, const Vector2f& direction, const float distance, const AABB2f& target, Intersect2f& out_Result);
	bool Sweep(const AABB2f& rect, const Vector2f& direction, const float distance, const Segment2f& target, Intersect2f& out_Result);
	bool Sweep(const AABB2f& rect, const Vector2f& direction, const float distance, const Triangle2f& target, Intersect2f& out_Result);

	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Circle2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Line2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Ray2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const AABB2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Segment2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Triangle2f& target, Intersect2f& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const AABB3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const Line3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const OBB3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const Ray3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const Segment3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const Sphere3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB3f& aabb, const Vector3f& direction, const float distance, const Triangle3f& target, Intersect3f& out_Result);

	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const AABB3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Line3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const OBB3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Ray3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Segment3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Sphere3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Triangle3f& target, Intersect3f& out_Result);
}