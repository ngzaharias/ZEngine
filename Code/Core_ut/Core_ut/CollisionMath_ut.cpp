#include <Catch2/catch.hpp>

#include <Core/AABB.h>
#include <Core/CollisionMath.h>
#include <Core/Line.h>
#include <Core/OBB.h>
#include <Core/Plane.h>
#include <Core/Ray.h>
#include <Core/Segment.h>
#include <Core/Sphere.h>

namespace
{
	constexpr Vector3f s_OBBExtents = Vector3f(0.5f);
}

TEST_CASE("math::Intersection(Segment, Plane)::Plane normal and Segment are perpendicular to each other.")
{
	Segment a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Segment, Plane)::Segment stops on the plane.")
{
	Segment a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
	b.m_Point = Vector3f(+1.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f(+1.f, 0.f, 0.f));
}

TEST_CASE("math::Intersection(Segment, Plane)::Segment stops short of the plane.")
{
	Segment a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
	b.m_Point = Vector3f(+1.01f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Segment, Plane)")
{
	Segment a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Ray, Plane)::Plane normal and Ray are perpendicular to each other.")
{
	Ray a; Plane b;
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Ray, Plane)::Plane normal and Ray are parallel to each other.")
{
	Ray a; Plane b;
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Ray, Plane)::Ray points in the opposite direction to the Plane normal.")
{
	Ray a; Plane b;
	a.m_Direction = Vector3f(-1.f, 0.f, 0.f);
	a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::IntersectionXZ(Ray, Ray)::Intersects at origin.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	Vector3f c;
	CHECK(math::IntersectionXZ(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::IntersectionXZ(Ray, Ray)::Intersects at origin, but one Ray is offset on the X axis.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-10.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	Vector3f c;
	CHECK(math::IntersectionXZ(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::IntersectionXZ(Ray, Ray)::Intersects at origin, but one Ray is offset on the Y axis.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, -1.f, 0.f);
	a.m_Direction = Vector3f(+1.f, -1.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, +1.f, -1.f);
	b.m_Direction = Vector3f(0.f, +1.f, +1.f);

	Vector3f c;
	CHECK(math::IntersectionXZ(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::IntersectionXZ(Ray, Ray)::Perpendicular Rays, but one Ray is in front of the other.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(+10.f, -1.f, 0.f);
	a.m_Direction = Vector3f(+1.f, -1.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, +1.f, -1.f);
	b.m_Direction = Vector3f(0.f, +1.f, +1.f);

	Vector3f c;
	CHECK(!math::IntersectionXZ(a, b, c));
}

TEST_CASE("math::IntersectionXZ(Ray, Ray)::Parallel Rays.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, 0.f, -1.f);
	a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
	b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
	b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

	Vector3f c;
	CHECK(!math::IntersectionXZ(a, b, c));
}

TEST_CASE("Parallel Rays, but one Ray is offset on the X axis.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-10.f, 0.f, -1.f);
	a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
	b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
	b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

	Vector3f c;
	CHECK(!math::IntersectionXZ(a, b, c));
}

TEST_CASE("math::IntersectionXZ(Segment, Segment)::Segments intersect at origin.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_PointA = Vector3f(0.f, 0.f, -1.f);
	b.m_PointB = Vector3f(0.f, 0.f, +1.f);

	Vector3f c;
	CHECK(math::IntersectionXZ(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::IntersectionXZ(Segment, Segment)::Segments intersect at origin with different Y.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_PointA = Vector3f(0.f, 0.f, -1.f);
	b.m_PointB = Vector3f(0.f, 0.f, +1.f);

	Vector3f c;
	CHECK(math::IntersectionXZ(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::IntersectionXZ(Segment, Segment)::Segments are separated by Z.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-1.f, 0.f, -1.f);
	a.m_PointB = Vector3f(+1.f, 0.f, -1.f);
	b.m_PointA = Vector3f(-1.f, 0.f, +1.f);
	b.m_PointB = Vector3f(+1.f, 0.f, +1.f);

	Vector3f c;
	CHECK(!math::IntersectionXZ(a, b, c));
}

TEST_CASE("math::IsAxisIntersecting(OBB, OBB, Axis)")
{
}

TEST_CASE("math::IsOverlapping(AABB, AABB)::A overlaps B.")
{
	AABB a, b;
	a.m_Min = Vector3f(0.0f);
	a.m_Max = Vector3f(1.0f);
	b.m_Min = Vector3f(0.5f);
	b.m_Max = Vector3f(1.5f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(AABB, AABB)::No overlap.")
{
	AABB a, b;
	a.m_Min = Vector3f(0.f);
	a.m_Max = Vector3f(1.f);
	b.m_Min = Vector3f(2.f);
	b.m_Max = Vector3f(3.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(AABB, AABB)::Share Corner.")
{
	AABB a, b;
	a.m_Min = Vector3f(0.f);
	a.m_Max = Vector3f(1.f);
	b.m_Min = Vector3f(1.f);
	b.m_Max = Vector3f(2.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(AABB, AABB)::Share Edge.")
{
	AABB a, b;
	a.m_Min = Vector3f(0.f);
	a.m_Max = Vector3f(1.f);
	b.m_Min = Vector3f(1.f, 0.f, 0.f);
	b.m_Max = Vector3f(1.f, 2.f, 2.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(AABB, AABB)::A Inside B.")
{
	AABB a, b;
	a.m_Min = Vector3f(-1.f);
	a.m_Max = Vector3f(+1.f);
	b.m_Min = Vector3f(-2.f);
	b.m_Max = Vector3f(+2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(AABB, AABB)::B Inside A.")
{
	AABB a, b;
	a.m_Min = Vector3f(-2.f);
	a.m_Max = Vector3f(+2.f);
	b.m_Min = Vector3f(-1.f);
	b.m_Max = Vector3f(+1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(AABB, Ray)::Ray touches edge.")
{
	constexpr AABB aabb = AABB(Vector3f::Zero, Vector3f::One);

	Ray ray;
	ray.m_Direction = -Vector3f::AxisX;
	ray.m_OriginPos = Vector3f(1.5f, 0.5f, 0.5f);
	CHECK(math::IsOverlapping(aabb, ray));
}

TEST_CASE("math::IsOverlapping(AABB, Ray)::Ray points in wrong direction.")
{
	constexpr AABB aabb = AABB(Vector3f::Zero, Vector3f::One);

	Ray ray;
	ray.m_Direction = Vector3f::AxisX;
	ray.m_OriginPos = Vector3f(1.5f, 0.5f, 0.5f);
	CHECK(!math::IsOverlapping(aabb, ray));
}

TEST_CASE("math::IsOverlapping(AABB, Ray)::Glancing Corner.")
{
	constexpr AABB aabb = AABB(Vector3f::Zero, Vector3f::One);

	Ray ray;
	ray.m_Direction = Vector3f::AxisZ;
	ray.m_OriginPos = Vector3f(1.f);
	CHECK(!math::IsOverlapping(aabb, ray));
}

TEST_CASE("math::IsOverlapping(AABB, Ray)::Glancing Edge.")
{
	constexpr AABB aabb = AABB(Vector3f::Zero, Vector3f::One);

	Ray ray;
	ray.m_Direction = Vector3f::AxisZ;
	ray.m_OriginPos = Vector3f(1.f, 0.5f, -1.f);
	CHECK(!math::IsOverlapping(aabb, ray));
}

TEST_CASE("math::IsOverlapping(OBB, OBB)::Collision, No Rotation.")
{
	OBB a, b;
	a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_OBBExtents);
	b = OBB::FromExtents(Vector3f(1.0f), Matrix3x3::Identity, s_OBBExtents);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(OBB, OBB)::No Collision, No Rotation.")
{
	OBB a, b;
	a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_OBBExtents);
	b = OBB::FromExtents(Vector3f(2.5f), Matrix3x3::Identity, s_OBBExtents);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(OBB, OBB)::Share Corner, No Rotation.")
{
	OBB a, b;
	a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_OBBExtents);
	b = OBB::FromExtents(Vector3f(1.5f), Matrix3x3::Identity, s_OBBExtents);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(OBB, OBB)::Share Edge, No Rotation.")
{
	OBB a, b;
	a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_OBBExtents);
	b = OBB::FromExtents(Vector3f(1.5f, 0.f, 0.f), Matrix3x3::Identity, s_OBBExtents);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(OBB, OBB)::A Inside B, No Rotation.")
{
	OBB a, b;
	a = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
	b = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f(2.f));
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(OBB, OBB)::B Inside A, No Rotation.")
{
	OBB a, b;
	a = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f(2.f));
	b = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Sphere)::Spheres touch each others centres.")
{
	Sphere a, b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector3f(1.f, 0.f, 0.f);
	b.m_Radius = 1.f;

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Sphere)::Spheres don't touch.")
{
	Sphere a, b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector3f(3.f, 0.f, 0.f);
	b.m_Radius = 1.f;

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Sphere)::Sphere with radius 0 touches.")
{
	Sphere a, b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector3f(0.f, 0.f, 0.f);
	b.m_Radius = 0.f;

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Sphere)::Spheres touch each others edges.")
{
	Sphere a, b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector3f(2.f, 0.f, 0.f);
	b.m_Radius = 1.f;

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Line)::Line intersects middle of Sphere.")
{
	Sphere a; Line b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector3f(0.f, 0.f, -2.f);
	b.m_PointB = Vector3f(0.f, 0.f, +2.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Line)::Line misses Sphere's edge.")
{
	Sphere a; Line b;
	a.m_Position = Vector3f(2.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector3f(0.f, 0.f, -1.f);
	b.m_PointB = Vector3f(0.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Line)::Line touches Sphere's edge.")
{
	Sphere a; Line b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector3f(1.f, 0.f, -1.f);
	b.m_PointB = Vector3f(1.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Ray)::Ray intersects middle of Sphere.")
{
	Sphere a; Ray b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Ray)::Ray intersects middle of Sphere from far away.")
{
	Sphere a; Ray b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_OriginPos = Vector3f(0.f, 0.f, -10.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Ray)::Ray misses Sphere's edge.")
{
	Sphere a; Ray b;
	a.m_Position = Vector3f(2.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Sphere, Ray)::Ray touches Sphere's edge.")
{
	Sphere a; Ray b;
	a.m_Position = Vector3f(0.f, 0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_OriginPos = Vector3f(1.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Perpendicular at Origin.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Perpendicular away from Origin.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-10.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Perpendicular with different Y.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, -1.f, 0.f);
	a.m_Direction = Vector3f(+1.f, -1.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, +1.f, -1.f);
	b.m_Direction = Vector3f(0.f, +1.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Perpendicular with overlapping origins.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f( 0.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, 0.f,  0.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Perpendicular with offset origins.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_OriginPos = Vector3f(0.f, 0.f, +1.f);
	b.m_Direction = Vector3f(0.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Parallel Lines.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-1.f, 0.f, -1.f);
	a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
	b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
	b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)::Parallel Lines With Further Origin.")
{
	Ray a, b;
	a.m_OriginPos = Vector3f(-10.f, 0.f, -1.f);
	a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
	b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
	b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Segment, Segment)::Perpendicular at Origin.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_PointA = Vector3f(0.f, 0.f, -1.f);
	b.m_PointB = Vector3f(0.f, 0.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Segment, Segment)::Perpendicular away from Origin.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-10.f, 0.f, 0.f);
	a.m_PointB = Vector3f(-0.1f, 0.f, 0.f);
	b.m_PointA = Vector3f(0.f, 0.f, -1.f);
	b.m_PointB = Vector3f(0.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Segment, Segment)::Perpendicular with different Y.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-1.f, -1.f, 0.f);
	a.m_PointB = Vector3f(+1.f, -1.f, 0.f);
	b.m_PointA = Vector3f(0.f, +1.f, -1.f);
	b.m_PointB = Vector3f(0.f, +1.f, +1.f);

	CHECK(math::IsOverlappingXZ(a, b));
}

TEST_CASE("math::IsOverlappingXZ(Segment, Segment)::Parallel Lines.")
{
	Segment a, b;
	a.m_PointA = Vector3f(-1.f, 0.f, -1.f);
	a.m_PointB = Vector3f(+1.f, 0.f, -1.f);
	b.m_PointA = Vector3f(-1.f, 0.f, +1.f);
	b.m_PointB = Vector3f(+1.f, 0.f, +1.f);

	CHECK(!math::IsOverlappingXZ(a, b));
}
