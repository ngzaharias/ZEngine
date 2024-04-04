#pragma once

#include <Core/Vector.h>

class AABB;
class Circle2f;
class Line2f;
class Line3f;
class OBB;
class Plane3f;
class Ray2f;
class Ray3f;
class Rect2f;
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

	bool Intersection(const Line3f& a, const Plane3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Ray3f& a, const Plane3f& b, Vector3f& out_IntersectPos);
	bool Intersection(const Segment3f& a, const Plane3f& b, Vector3f& out_IntersectPos);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	/// \brief Returns true if Circle A is overlapping Circle B.
	bool IsOverlapping(const Circle2f& a, const Circle2f& b);
	/// \brief Returns true if a Circle is overlapping a Line.
	bool IsOverlapping(const Circle2f& a, const Line2f& b);
	/// \brief Returns true if a Circle is overlapping a Ray.
	bool IsOverlapping(const Circle2f& a, const Ray2f& b);
	/// \brief Returns true if a Circle is overlapping a Rect.
	bool IsOverlapping(const Circle2f& a, const Rect2f& b);
	/// \brief Returns true if a Circle is overlapping a Segment.
	bool IsOverlapping(const Circle2f& a, const Segment2f& b);
	/// \brief Returns true if a Circle is overlapping a Triangle.
	bool IsOverlapping(const Circle2f& a, const Triangle2f& b);
	/// \brief Returns true if a Circle is overlapping a Point.
	bool IsOverlapping(const Circle2f& a, const Vector2f& b);

	/// \brief Returns true if a Line is overlapping a Circle.
	bool IsOverlapping(const Line2f& a, const Circle2f& b);
	/// \brief Returns true if Line A is overlapping Line B.
	bool IsOverlapping(const Line2f& a, const Line2f& b);
	/// \brief Returns true if a Line is overlapping a Ray.
	bool IsOverlapping(const Line2f& a, const Ray2f& b);
	/// \brief Returns true if a Line is overlapping a Rect.
	bool IsOverlapping(const Line2f& a, const Rect2f& b);
	/// \brief Returns true if a Line is overlapping a Segment.
	bool IsOverlapping(const Line2f& a, const Segment2f& b);
	/// \brief Returns true if a Line is overlapping a Triangle.
	bool IsOverlapping(const Line2f& a, const Triangle2f& b);

	/// \brief Returns true if a Ray is overlapping a Circle.
	bool IsOverlapping(const Ray2f& a, const Circle2f& b);
	/// \brief Returns true if a Ray is overlapping a Line.
	bool IsOverlapping(const Ray2f& a, const Line2f& b);
	/// \brief Returns true if Ray A is overlapping Ray B.
	bool IsOverlapping(const Ray2f& a, const Ray2f& b);
	/// \brief Returns true if a Ray is overlapping a Rect.
	bool IsOverlapping(const Ray2f& a, const Rect2f& b);
	/// \brief Returns true if a Ray is overlapping a Segment.
	bool IsOverlapping(const Ray2f& a, const Segment2f& b);
	/// \brief Returns true if a Ray is overlapping a Triangle.
	bool IsOverlapping(const Ray2f& a, const Triangle2f& b);

	/// \brief Returns true if a Rect is overlapping a Circle.
	bool IsOverlapping(const Rect2f& a, const Circle2f& b);
	/// \brief Returns true if a Rect is overlapping a Line.
	bool IsOverlapping(const Rect2f& a, const Line2f& b);
	/// \brief Returns true if a Rect is overlapping a Ray.
	bool IsOverlapping(const Rect2f& a, const Ray2f& b);
	/// \brief Returns true if Rect A is overlapping Rect B.
	bool IsOverlapping(const Rect2f& a, const Rect2f& b);
	/// \brief Returns true if a Rect is overlapping a Segment.
	bool IsOverlapping(const Rect2f& a, const Segment2f& b);
	/// \brief Returns true if a Rect is overlapping a Triangle.
	bool IsOverlapping(const Rect2f& a, const Triangle2f& b);
	/// \brief Returns true if a Rect is overlapping a Point.
	bool IsOverlapping(const Rect2f& a, const Vector2f& b);

	/// \brief Returns true if a Segment is overlapping a Circle.
	bool IsOverlapping(const Segment2f& a, const Circle2f& b);
	/// \brief Returns true if a Segment is overlapping a Line.
	bool IsOverlapping(const Segment2f& a, const Line2f& b);
	/// \brief Returns true if a Segment is overlapping a Ray.
	bool IsOverlapping(const Segment2f& a, const Ray2f& b);
	/// \brief Returns true if a Segment is overlapping a Rect.
	bool IsOverlapping(const Segment2f& a, const Rect2f& b);
	/// \brief Returns true if Segment A is overlapping Segment B.
	bool IsOverlapping(const Segment2f& a, const Segment2f& b);
	/// \brief Returns true if a Segment is overlapping a Triangle.
	bool IsOverlapping(const Segment2f& a, const Triangle2f& b);

	/// \brief Returns true if a Triangle is overlapping a Circle.
	bool IsOverlapping(const Triangle2f& a, const Circle2f& b);
	/// \brief Returns true if a Triangle is overlapping a Line.
	bool IsOverlapping(const Triangle2f& a, const Line2f& b);
	/// \brief Returns true if a Triangle is overlapping a Ray.
	bool IsOverlapping(const Triangle2f& a, const Ray2f& b);
	/// \brief Returns true if a Triangle is overlapping a Rect.
	bool IsOverlapping(const Triangle2f& a, const Rect2f& b);
	/// \brief Returns true if a Triangle is overlapping a Segment.
	bool IsOverlapping(const Triangle2f& a, const Segment2f& b);
	/// \brief Returns true if Triangle A is overlapping Triangle B.
	bool IsOverlapping(const Triangle2f& a, const Triangle2f& b);
	/// \brief Returns true if a Triangle is overlapping a Point.
	bool IsOverlapping(const Triangle2f& a, const Vector2f& b);

	/// \brief Returns true if a Point is overlapping a Circle.
	bool IsOverlapping(const Vector2f& a, const Circle2f& b);
	/// \brief Returns true if a Point is overlapping a Rect.
	bool IsOverlapping(const Vector2f& a, const Rect2f& b);
	/// \brief Returns true if a Point is overlapping a Triangle.
	bool IsOverlapping(const Vector2f& a, const Triangle2f& b);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool IsOverlapping(const AABB& a, const AABB& b);
	bool IsOverlapping(const AABB& a, const Line3f& b);
	bool IsOverlapping(const AABB& a, const OBB& b);
	bool IsOverlapping(const AABB& a, const Plane3f& b);
	bool IsOverlapping(const AABB& a, const Ray3f& b);
	bool IsOverlapping(const AABB& a, const Segment3f& b);
	bool IsOverlapping(const AABB& a, const Sphere3f& b);

	bool IsOverlapping(const Line3f& a, const AABB& b);
	bool IsOverlapping(const Line3f& a, const OBB& b);
	bool IsOverlapping(const Line3f& a, const Plane3f& b);
	bool IsOverlapping(const Line3f& a, const Ray3f& b);
	bool IsOverlapping(const Line3f& a, const Segment3f& b);
	bool IsOverlapping(const Line3f& a, const Sphere3f& b);

	bool IsOverlapping(const OBB& a, const AABB& b);
	bool IsOverlapping(const OBB& a, const Line3f& b);
	bool IsOverlapping(const OBB& a, const OBB& b);
	bool IsOverlapping(const OBB& a, const Plane3f& b);
	bool IsOverlapping(const OBB& a, const Ray3f& b);
	bool IsOverlapping(const OBB& a, const Segment3f& b);
	bool IsOverlapping(const OBB& a, const Sphere3f& b);

	bool IsOverlapping(const Plane3f& a, const AABB& b);
	bool IsOverlapping(const Plane3f& a, const Line3f& b);
	bool IsOverlapping(const Plane3f& a, const OBB& b);
	bool IsOverlapping(const Plane3f& a, const Plane3f& b);
	bool IsOverlapping(const Plane3f& a, const Ray3f& b);
	bool IsOverlapping(const Plane3f& a, const Segment3f& b);
	bool IsOverlapping(const Plane3f& a, const Sphere3f& b);

	bool IsOverlapping(const Ray3f& a, const AABB& b);
	bool IsOverlapping(const Ray3f& a, const Line3f& b);
	bool IsOverlapping(const Ray3f& a, const OBB& b);
	bool IsOverlapping(const Ray3f& a, const Plane3f& b);
	bool IsOverlapping(const Ray3f& a, const Ray3f& b);
	bool IsOverlapping(const Ray3f& a, const Segment3f& b);
	bool IsOverlapping(const Ray3f& a, const Sphere3f& b);

	bool IsOverlapping(const Segment3f& a, const AABB& b);
	bool IsOverlapping(const Segment3f& a, const Line3f& b);
	bool IsOverlapping(const Segment3f& a, const OBB& b);
	bool IsOverlapping(const Segment3f& a, const Plane3f& b);
	bool IsOverlapping(const Segment3f& a, const Ray3f& b);
	bool IsOverlapping(const Segment3f& a, const Segment3f& b);
	bool IsOverlapping(const Segment3f& a, const Sphere3f& b);

	bool IsOverlapping(const Sphere3f& a, const AABB& b);
	bool IsOverlapping(const Sphere3f& a, const Line3f& b);
	bool IsOverlapping(const Sphere3f& a, const OBB& b);
	bool IsOverlapping(const Sphere3f& a, const Plane3f& b);
	bool IsOverlapping(const Sphere3f& a, const Ray3f& b);
	bool IsOverlapping(const Sphere3f& a, const Segment3f& b);
	bool IsOverlapping(const Sphere3f& a, const Sphere3f& b);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	bool Raycast(const Ray2f& ray, const float distance, const Circle2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Line2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Ray2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Rect2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Segment2f& target, Intersect2f& out_Result);
	bool Raycast(const Ray2f& ray, const float distance, const Triangle2f& target, Intersect2f& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool Raycast(const Ray3f& ray, const float distance, const AABB& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const Plane3f& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const OBB& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const Sphere3f& target, Intersect3f& out_Result);
	bool Raycast(const Ray3f& ray, const float distance, const Triangle3f& target, Intersect3f& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 2D

	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Circle2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Line2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Ray2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Rect2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Segment2f& target, Intersect2f& out_Result);
	bool Sweep(const Circle2f& circle, const Vector2f& direction, const float distance, const Triangle2f& target, Intersect2f& out_Result);

	bool Sweep(const Rect2f& rect, const Vector2f& direction, const float distance, const Circle2f& target, Intersect2f& out_Result);
	bool Sweep(const Rect2f& rect, const Vector2f& direction, const float distance, const Line2f& target, Intersect2f& out_Result);
	bool Sweep(const Rect2f& rect, const Vector2f& direction, const float distance, const Ray2f& target, Intersect2f& out_Result);
	bool Sweep(const Rect2f& rect, const Vector2f& direction, const float distance, const Rect2f& target, Intersect2f& out_Result);
	bool Sweep(const Rect2f& rect, const Vector2f& direction, const float distance, const Segment2f& target, Intersect2f& out_Result);
	bool Sweep(const Rect2f& rect, const Vector2f& direction, const float distance, const Triangle2f& target, Intersect2f& out_Result);

	//////////////////////////////////////////////////////////////////////////
	// 3D

	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const AABB& target, Intersect3f& out_Result);
	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const Line3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const OBB& target, Intersect3f& out_Result);
	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const Ray3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const Segment3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const Sphere3f& target, Intersect3f& out_Result);
	bool Sweep(const AABB& aabb, const Vector3f& direction, const float distance, const Triangle3f& target, Intersect3f& out_Result);

	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const AABB& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Line3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const OBB& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Ray3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Segment3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Sphere3f& target, Intersect3f& out_Result);
	bool Sweep(const Sphere3f& sphere, const Vector3f& direction, const float distance, const Triangle3f& target, Intersect3f& out_Result);
}