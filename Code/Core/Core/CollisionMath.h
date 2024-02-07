#pragma once

#include <Core/Vector.h>

class AABB;
class Line;
class OBB;
class Plane;
class Ray;
class Segment;
class Sphere;

namespace math
{
	struct Intersect
	{
		/// \brief The contact point between the two objects.
		Vector3f m_HitPoint;
		/// \brief The normal of the face that was hit.
		Vector3f m_HitNormal;
		/// \brief Fraction along source and target where the hit happened.
		float m_HitFraction;
	};

	bool AreCounterClockwiseOrderedXZ(const Vector3f& a, const Vector3f& b, const Vector3f& c);

	/// \brief Checks if an AABB intersects a Line and returns the IntersectPos if it does.
	bool Intersection(const Segment& segment, const AABB& aabb, Vector3f& out_IntersectPos);
	/// \brief Checks if a Line intersects a Plane and returns the IntersectPos if it does.
	bool Intersection(const Segment& segment, const Plane& plane, Vector3f& out_IntersectPos);
	/// \brief Checks if a Ray intersects a Plane and returns the IntersectPos if it does.
	bool Intersection(const Ray& ray, const Plane& plane, Vector3f& out_IntersectPos);

	/// \brief Checks if a Ray intersects another Ray in the XZ plane and returns the IntersectPos if it does.
	bool IntersectionXZ(const Ray& a, const Ray& b, Vector3f& out_IntersectPos);
	/// \brief Checks if a Line Segments intersects another Line Segment in the XZ plane and returns the IntersectPos if it does.
	bool IntersectionXZ(const Segment& a, const Segment& b, Vector3f& out_IntersectPos);

	/// \brief Checks if a Sphere intersects a Ray when projected along a vector in the XZ plane and returns the IntersectPos if it does.
	bool IntersectionXZ(const Sphere& sphere, const Vector3f& direction, const float distance, const Ray& ray, Intersect& out_Result);
	/// \brief Checks if a Sphere intersects a Line Segment when projected along a vector in the XZ plane and returns the IntersectPos if it does.
	bool IntersectionXZ(const Sphere& sphere, const Vector3f& direction, const float distance, const Segment& segment, Intersect& out_Result);

	/// \brief Checks if two Orientated Bounding Boxes are overlapping each other using Separating Axis Theorem on a single axis.
	bool IsAxisOverlapping(const OBB& a, const OBB& b, const Vector3f& axis);

	/// \brief Checks if two Axis Aligned Bounding Boxes are overlapping each other.
	bool IsOverlapping(const AABB& a, const AABB& b);
	/// \brief Checks in an Axis Aligned Bounding Box and a Ray are overlapping.
	bool IsOverlapping(const AABB& aabb, const Ray& ray);
	/// \brief Checks if two Orientated Bounding Boxes are overlapping each other using Separating Axis Theorem.
	bool IsOverlapping(const OBB& a, const OBB& b);
	/// \brief Checks if two Spheres are overlapping on the XZ plane.
	bool IsOverlappingXZ(const Sphere& a, const Sphere& b);
	/// \brief Checks if a Sphere and a line segment are overlapping on the XZ plane.
	bool IsOverlappingXZ(const Sphere& sphere, const Line& line);
	/// \brief Checks if a Sphere and a ray are overlapping on the XZ plane.
	bool IsOverlappingXZ(const Sphere& sphere, const Ray& ray);
	/// \brief Checks if two Rays are overlapping on the XZ plane.
	bool IsOverlappingXZ(const Ray& a, const Ray& b);
	/// \brief Checks if two Line segments are overlapping on the XZ plane.
	bool IsOverlappingXZ(const Segment& a, const Segment& b);
}