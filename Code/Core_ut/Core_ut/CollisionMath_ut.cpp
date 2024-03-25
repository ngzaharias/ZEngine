
#include <Catch2/catch.hpp>

#include <Core/AABB.h>
#include <Core/Circle.h>
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

TEST_CASE("math::Intersection(Plane Line3f). Line intersects with points either side of the Plane at Origin.")
{
	Line3f a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Plane Line3f). Line intersects with points either side of the Plane but not at Origin.")
{
	Line3f a; Plane b;
	a.m_PointA = Vector3f(-KINDA_LARGE_FLOAT, 0.f, 0.f);
	a.m_PointB = Vector3f(+KINDA_LARGE_FLOAT, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(-1100.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f(-1100.f, 0.f, 0.f));
}

TEST_CASE("math::Intersection(Plane Line3f). Line intersects with points on the left side of the Plane.")
{
	Line3f a; Plane b;
	a.m_PointA = Vector3f(-2.f, 0.f, 0.f);
	a.m_PointB = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Plane Line3f). Line intersects with points on the right side of the Plane.")
{
	Line3f a; Plane b;
	a.m_PointA = Vector3f(+1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+2.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Plane Line3f). Line intersects Plane when Plane normal and Line direction are parallel to each other.")
{
	Line3f a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Plane Ray3f). Ray intersects when starting on the left side of the Plane but points right.")
{
	Ray3f a; Plane b;
	a.m_Position = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Plane Ray3f). Ray intersects when starting on the left side of the Plane but points left.")
{
	Ray3f a; Plane b;
	a.m_Position = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Plane Ray3f). Ray doesn't intersect when starting on the right side of the Plane but points right.")
{
	Ray3f a; Plane b;
	a.m_Position = Vector3f(+1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Plane Ray3f). Ray intersects when starting on the right side of the Plane but points left.")
{
	Ray3f a; Plane b;
	a.m_Position = Vector3f(+1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Plane Ray3f). Ray doesn't intersect when Plane normal and Ray direction are perpendicular to each other.")
{
	Ray3f a; Plane b;
	a.m_Position = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Plane Segment3f). Segment intersects when points are either side of the Plane.")
{
	Segment3f a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Plane Segment3f). Segment doesn't intersect when points are on the left side of the Plane.")
{
	Segment3f a; Plane b;
	a.m_PointA = Vector3f(-2.f, 0.f, 0.f);
	a.m_PointB = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Plane Segment3f). Segment doesn't intersect when points are on the right side of the Plane.")
{
	Segment3f a; Plane b;
	a.m_PointA = Vector3f(+1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+2.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Plane Segment3f). Segment doesn't intersect when Segment points and Plane normal are perpendicular to each other.")
{
	Segment3f a; Plane b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles touch each others centres.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(1.f, 0.f);
	b.m_Radius = 1.f;

	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles don't touch.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(3.f, 0.f);
	b.m_Radius = 1.f;

	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles with radius 0 touches.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(0.f, 0.f);
	b.m_Radius = 0.f;

	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles touch each others edges.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(2.f, 0.f);
	b.m_Radius = 1.f;

	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Line2f). Circle and Line overlap when line runs through centre of circle.")
{
	Circle2f a; Line2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);

	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Line2f). Circle and Line don't overlap when the line misses the circle's edge.")
{
	Circle2f a; Line2f b;
	a.m_Position = Vector2f(2.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);

	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Line2f). Circle and Line overlap when the line touches the circle's edge.")
{
	Circle2f a; Line2f b;
	a.m_Position = Vector2f(1.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);

	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Ray2f). Circle and Ray overlap when ray runs through centre of circle.")
{
	Circle2f a; Ray2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(0.f, -2.f);
	b.m_Direction = Vector2f(0.f, +1.f);

	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Ray2f). Circle and Ray don't overlap when ray points in the opposite direction.")
{
	Circle2f a; Ray2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(0.f, -2.f);
	b.m_Direction = Vector2f(0.f, -1.f);

	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Ray2f). Circle and Ray don't overlap when the ray misses the circle's edge.")
{
	Circle2f a; Ray2f b;
	a.m_Position = Vector2f(2.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(0.f, -2.f);
	b.m_Direction = Vector2f(0.f, +1.f);

	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Ray2f). Circle and Ray overlap when the ray touches the circle's edge.")
{
	Circle2f a; Ray2f b;
	a.m_Position = Vector2f(1.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(0.f, -2.f);
	b.m_Direction = Vector2f(0.f, +1.f);

	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). ")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Circle2f Segment2f). ")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Line2f Circle2f). Line and Circle overlap when line runs through centre of circle.")
{
	// math::IsOverlapping(Circle2f Line2f).
}

TEST_CASE("math::IsOverlapping(Line2f Line2f). Lines overlap when they are perpendicular at Origin.")
{
	Line2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Line2f). Lines overlap when they are perpendicular but not at Origin.")
{
	Line2f a, b;
	a.m_PointA = Vector2f(-999.f, 100.f);
	a.m_PointB = Vector2f(-998.f, 100.f);
	b.m_PointA = Vector2f(100.f, -1.f);
	b.m_PointB = Vector2f(100.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Line2f). Lines don't overlap when they are parallel.")
{
	Line2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 1.f);
	b.m_PointB = Vector2f(+1.f, 1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Line2f). Lines don't overlap when they are identical.")
{
	Line2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray overlap when they are perpendicular at Origin.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(0.f, -1.f);
	b.m_Direction = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray overlap when they are perpendicular but not at Origin.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(-999.f, 100.f);
	a.m_PointB = Vector2f(-998.f, 100.f);
	b.m_Position = Vector2f(100.f, -1.f);
	b.m_Direction = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray don't overlap when they are perpendicular but Ray points downwards.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(0.f, -1.f);
	b.m_Direction = Vector2f(0.f, -1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray don't overlap when they are perpendicular but Ray is above.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(0.f, +1.f);
	b.m_Direction = Vector2f(0.f, +1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray don't overlap when they are parallel.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(-1.f, 1.f);
	b.m_Direction = Vector2f(1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray don't overlap when they are parallel on top of each other.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(-1.f, 0.f);
	b.m_Direction = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray overlap when they cross at an angle and they are far away from each other.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(+999998.f, 0.f);
	a.m_PointB = Vector2f(+999999.f, 0.f);
	b.m_Position = Vector2f(-99999999.f);
	b.m_Direction = Vector2f(0.9999f, 0.0001f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Ray2f). Line and Ray don't overlap when floating point precision is a factor.")
{
	Line2f a; Ray2f b;
	a.m_PointA = Vector2f(+99999999998.f, 0.f);
	a.m_PointB = Vector2f(+99999999999.f, 0.f);
	b.m_Position = Vector2f(-9999999999999.f);
	b.m_Direction = Vector2f(0.9999f, 0.0001f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Rect2f). ")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment overlap when they are perpendicular at Origin.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment overlap when they are perpendicular and their edge's touch.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, 0.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment overlap when they are perpendicular not at Origin.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-999.f, 0.f);
	a.m_PointB = Vector2f(-998.f, 0.f);
	b.m_PointA = Vector2f(100.f, -1.f);
	b.m_PointB = Vector2f(100.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment overlap when they are perpendicular at Origin but points are flipped.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, +1.f);
	b.m_PointB = Vector2f(0.f, -1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when they are perpendicular but Segment is below Line.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, -10.f);
	b.m_PointB = Vector2f(0.f, -11.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when they are parallel.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 1.f);
	b.m_PointB = Vector2f(+1.f, 1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when they are parallel on top of each other.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment overlap when they overlap at an 45 degree angle.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-100.f, -1.f);
	a.m_PointB = Vector2f(+100.f, +1.f);
	b.m_PointA = Vector2f(-100.f, 0.f);
	b.m_PointB = Vector2f(+100.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment overlap when the Segments first point touches the Line.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, 0.f);
	b.m_PointB = Vector2f(0.f, 1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Circle2f).")
{
	// math::IsOverlapping(Circle2f Ray2f).
}

TEST_CASE("math::IsOverlapping(Ray2f Line2f).")
{
	// math::IsOverlapping(Line2f Ray2f).
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f).")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays overlap when they are perpendicular at Origin.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-1.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(0.f, -1.f);
	b.m_Direction = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays overlap when they are perpendicular but not at Origin.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-999.f, 100.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(999.f, -1.f);
	b.m_Direction = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays overlap when they are perpendicular and origin pos is shared.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-1.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(-1.f, 0.f);
	b.m_Direction = Vector2f(0.f, 1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays overlap when they are perpendicular and they share an edge.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-1.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(+10.f, 0.f);
	b.m_Direction = Vector2f(0.f, 1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays don't overlap when they are perpendicular but direction is opposite.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-1.f, +1.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(-1.f, -1.f);
	b.m_Direction = Vector2f(0.f, -1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays don't overlap when they are parallel.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-1.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(-1.f, 1.f);
	b.m_Direction = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays don't overlap when they are parallel on top of each other.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-2.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(-1.f, 0.f);
	b.m_Direction = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Ray2f). Rays don't overlap when they are parallel on top of each other, but directions are opposite.")
{
	Ray2f a, b;
	a.m_Position = Vector2f(-1.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Position = Vector2f(+1.f, 0.f);
	b.m_Direction = Vector2f(-1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). ")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment overlap when they are perpendicular at Origin.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(0.f, -1.f);
	a.m_Direction = Vector2f(0.f, +1.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment overlap when when they are perpendicular and their edge's touch.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Direction = Vector2f(0.f, 1.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment don't overlap when Ray is beside the Segment.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(2.f, -1.f);
	a.m_Direction = Vector2f(0.f, +1.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment overlap when they are perpendicular but not at Origin.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(100.f, -1.f);
	a.m_Direction = Vector2f(0.f, +1.f);
	b.m_PointA = Vector2f(-100.f, 100.f);
	b.m_PointB = Vector2f(+100.f, 100.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment don't overlap when they are perpendicular but Ray points downwards.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(0.f, -1.f);
	a.m_Direction = Vector2f(0.f, -1.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment don't overlap when they are perpendicular but Ray is above.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(0.f, +1.f);
	a.m_Direction = Vector2f(0.f, +1.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment don't overlap when they are parallel.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(-1.f, 1.f);
	a.m_Direction = Vector2f(1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment don't overlap when they are parallel on top of each other.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(-1.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment overlap when they cross at an angle and they are far away from each other.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(-9999.f, -1.f);
	a.m_Direction = Vector2f(0.9999f, 0.0001f);
	b.m_PointA = Vector2f(-999999.f, 0.f);
	b.m_PointB = Vector2f(+999999.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Segment2f). Ray and Segment don't overlap when floating point precision is a factor.")
{
	Ray2f a; Segment2f b;
	a.m_Position = Vector2f(-9999999999999.f);
	a.m_Direction = Vector2f(0.9999f, 0.0001f);
	b.m_PointA = Vector2f(-99999999999.f, 0.f);
	b.m_PointB = Vector2f(+99999999999.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Circle2f). ")
{
	// math::IsOverlapping(Circle2f Rect2f).
}

TEST_CASE("math::IsOverlapping(Rect2f Line2f). ")
{
	// math::IsOverlapping(Line2f Rect2f).
}

TEST_CASE("math::IsOverlapping(Rect2f Ray2f). ")
{
	// math::IsOverlapping(Ray2f Rect2f).
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). ")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). ")
{
	// #todo
}

TEST_CASE("math::IsOverlapping(Segment2f Circle2f).")
{
	// math::IsOverlapping(Circle2f Segment2f).
}

TEST_CASE("math::IsOverlapping(Segment2f Line2f).")
{
	// math::IsOverlapping(Line2f Segment2f).
}

TEST_CASE("math::IsOverlapping(Segment2f Ray2f).")
{
	// math::IsOverlapping(Ray2f Segment2f).
}

TEST_CASE("math::IsOverlapping(Segment2f Rect2f). ")
{
	// math::IsOverlapping(Rect2f Segment2f).
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments overlap when they are perpendicular at Origin.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments overlap when they are perpendicular and their edges touch.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f,  0.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments overlap when they are perpendicular not at Origin.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-999.f, 0.f);
	a.m_PointB = Vector2f(+999.f, 0.f);
	b.m_PointA = Vector2f(100.f, -1.f);
	b.m_PointB = Vector2f(100.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments overlap when they are perpendicular at Origin but points are flipped.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, +1.f);
	b.m_PointB = Vector2f(0.f, -1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments don't overlap when they are perpendicular but Segment is below Line.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, -10.f);
	b.m_PointB = Vector2f(0.f, -11.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments don't overlap when they are parallel.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 1.f);
	b.m_PointB = Vector2f(+1.f, 1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments don't overlap when they are parallel on top of each other.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments overlap when they overlap at an 45 degree angle.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-100.f, -1.f);
	a.m_PointB = Vector2f(+100.f, +1.f);
	b.m_PointA = Vector2f(-100.f, 0.f);
	b.m_PointB = Vector2f(+100.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Segment2f Segment2f). Segments overlap when the Segments first point touches the Line.")
{
	Segment2f a, b;
	a.m_PointA = Vector2f(-1.f, 0.f);
	a.m_PointB = Vector2f(+1.f, 0.f);
	b.m_PointA = Vector2f(0.f, 0.f);
	b.m_PointB = Vector2f(0.f, 1.f);
	CHECK(math::IsOverlapping(a, b));
}