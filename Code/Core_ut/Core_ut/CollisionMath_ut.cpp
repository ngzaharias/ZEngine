
#include <Catch2/catch.hpp>

#include <Core/AABB.h>
#include <Core/Circle.h>
#include <Core/CollisionMath.h>
#include <Core/Line.h>
#include <Core/OBB.h>
#include <Core/Plane.h>
#include <Core/Ray.h>
#include <Core/Rect.h>
#include <Core/Segment.h>
#include <Core/Sphere.h>
#include <Core/Triangle.h>

TEST_CASE("math::Intersection(Line3f Plane3f). Line intersects with points either side of the Plane at Origin.")
{
	Line3f a; Plane3f b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Line3f Plane3f). Line intersects with points either side of the Plane but not at Origin.")
{
	Line3f a; Plane3f b;
	a.m_PointA = Vector3f(-KINDA_LARGE_FLOAT, 0.f, 0.f);
	a.m_PointB = Vector3f(+KINDA_LARGE_FLOAT, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(-1100.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f(-1100.f, 0.f, 0.f));
}

TEST_CASE("math::Intersection(Line3f Plane3f). Line intersects with points on the left side of the Plane.")
{
	Line3f a; Plane3f b;
	a.m_PointA = Vector3f(-2.f, 0.f, 0.f);
	a.m_PointB = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Line3f Plane3f). Line intersects with points on the right side of the Plane.")
{
	Line3f a; Plane3f b;
	a.m_PointA = Vector3f(+1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+2.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Line3f Plane3f). Line intersects Plane when Plane normal and Line direction are parallel to each other.")
{
	Line3f a; Plane3f b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Ray3f Plane3f). Ray intersects when starting on the left side of the Plane but points right.")
{
	Ray3f a; Plane3f b;
	a.m_Position = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Ray3f Plane3f). Ray intersects when starting on the left side of the Plane but points left.")
{
	Ray3f a; Plane3f b;
	a.m_Position = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Ray3f Plane3f). Ray doesn't intersect when starting on the right side of the Plane but points right.")
{
	Ray3f a; Plane3f b;
	a.m_Position = Vector3f(+1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Ray3f Plane3f). Ray intersects when starting on the right side of the Plane but points left.")
{
	Ray3f a; Plane3f b;
	a.m_Position = Vector3f(+1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Ray3f Plane3f). Ray doesn't intersect when Plane normal and Ray direction are perpendicular to each other.")
{
	Ray3f a; Plane3f b;
	a.m_Position = Vector3f(-1.f, 0.f, 0.f);
	a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Segment3f Plane3f). Segment intersects when points are either side of the Plane.")
{
	Segment3f a; Plane3f b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(math::Intersection(a, b, c));
	CHECK(c == Vector3f::Zero);
}

TEST_CASE("math::Intersection(Segment3f Plane3f). Segment doesn't intersect when points are on the left side of the Plane.")
{
	Segment3f a; Plane3f b;
	a.m_PointA = Vector3f(-2.f, 0.f, 0.f);
	a.m_PointB = Vector3f(-1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Segment3f Plane3f). Segment doesn't intersect when points are on the right side of the Plane.")
{
	Segment3f a; Plane3f b;
	a.m_PointA = Vector3f(+1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+2.f, 0.f, 0.f);
	b.m_Normal = Vector3f(1.f, 0.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::Intersection(Segment3f Plane3f). Segment doesn't intersect when Segment points and Plane normal are perpendicular to each other.")
{
	Segment3f a; Plane3f b;
	a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
	a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
	b.m_Normal = Vector3f(0.f, 1.f, 0.f);
	b.m_Point = Vector3f(0.f, 0.f, 0.f);

	Vector3f c;
	CHECK(!math::Intersection(a, b, c));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles overlap when their edges touch each others centres.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(1.f, 0.f);
	b.m_Radius = 1.f;
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles don't overlap when they are separated.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(3.f, 0.f);
	b.m_Radius = 1.f;
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles overlap when one circle with 0 radius is enclosed by the other.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(0.f, 0.f);
	b.m_Radius = 0.f;
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Circle2f). Circles overlap when they touch each others edges.")
{
	Circle2f a, b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Position = Vector2f(2.f, 0.f);
	b.m_Radius = 1.f;
	CHECK(math::IsOverlapping(a, b));
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

TEST_CASE("math::IsOverlapping(Circle2f Line2f). Circle and Line overlap when line runs through centre of circle but points aren't either side.")
{
	Circle2f a; Line2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -4.f);
	b.m_PointB = Vector2f(0.f, -5.f);
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
	b.m_Position = Vector2f(+0.f, -2.f);
	b.m_Direction = Vector2f(+0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect overlap when the rect min in inside the circle.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(+0.f, +0.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect overlap when the rect min in inside the circle away from origin.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(+998.f, +998.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(+998.f, +998.f);
	b.m_Max = Vector2f(+999.f, +999.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect don't overlap when the rect min is outside the circle.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(+1.f, +1.f);
	b.m_Max = Vector2f(+2.f, +2.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect overlap when the rect max in inside the circle.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+0.f, +0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect don't overlap when the rect max in outside the circle.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(-2.f, -2.f);
	b.m_Max = Vector2f(-1.f, -1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect overlap when the rect touches the edge of the circle.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(+1.f, +0.f);
	b.m_Max = Vector2f(+2.f, +2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect overlap when the rect envelops the circle.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_Min = Vector2f(-2.f, -2.f);
	b.m_Max = Vector2f(+2.f, +2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Rect2f). Circle and Rect overlap when the circle envelops the rect.")
{
	Circle2f a; Rect2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 4.f;
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Segment2f). Circle and Segment overlap when segment runs through centre of circle.")
{
	Circle2f a; Segment2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Segment2f). Circle and Segment don't overlap when segment stops short of circle.")
{
	Circle2f a; Segment2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, -4.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Segment2f). Circle and Segment don't overlap when the segment misses the circle's edge.")
{
	Circle2f a; Segment2f b;
	a.m_Position = Vector2f(2.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Segment2f). Circle and Segment overlap when the segment touches the circle's edge.")
{
	Circle2f a; Segment2f b;
	a.m_Position = Vector2f(1.f, 0.f);
	a.m_Radius = 1.f;
	b.m_PointA = Vector2f(0.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Vector2f). Circle and Point overlap when the point is enclosed by the circle.")
{
	Circle2f a; Vector2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b = Vector2f(0.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Vector2f). Circle and Point don't overlap when the point is outside the circle.")
{
	Circle2f a; Vector2f b;
	a.m_Position = Vector2f(0.f, 0.f);
	a.m_Radius = 1.f;
	b = Vector2f(2.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Circle2f Vector2f). Circle and Point overlap when the point touches the circle's edge.")
{
	Circle2f a; Vector2f b;
	a.m_Position = Vector2f(1.f, 0.f);
	a.m_Radius = 1.f;
	b = Vector2f(0.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
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

TEST_CASE("math::IsOverlapping(Line2f Rect2f). Line and Rect overlap when line runs through the centre of the rect.")
{
	Line2f a; Rect2f b;
	a.m_PointA = Vector2f(-2.f, 0.f);
	a.m_PointB = Vector2f(+2.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Rect2f). Line and Rect overlap when line runs through the centre of the rect away from origin.")
{
	Line2f a; Rect2f b;
	a.m_PointA = Vector2f(-96.f, +97.f);
	a.m_PointB = Vector2f(+99.f, +97.f);
	b.m_Min = Vector2f(+97.f, +96.f);
	b.m_Max = Vector2f(+98.f, +98.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Rect2f). Line and Rect overlap when line runs through the centre of the rect but points aren't either side.")
{
	Line2f a; Rect2f b;
	a.m_PointA = Vector2f(-4.f, 0.f);
	a.m_PointB = Vector2f(-2.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Rect2f). Line and Rect don't overlap when line misses the rect.")
{
	Line2f a; Rect2f b;
	a.m_PointA = Vector2f(-2.f, -2.f);
	a.m_PointB = Vector2f(+2.f, -2.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(!math::IsOverlapping(a, b)); 
}

TEST_CASE("math::IsOverlapping(Line2f Rect2f). Line and Rect overlap when line touches the edge of the rect.")
{
	Line2f a; Rect2f b;
	a.m_PointA = Vector2f(-2.f, -1.f);
	a.m_PointB = Vector2f(+2.f, -1.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Rect2f). Line and Rect overlap when line touches the corner of the rect.")
{
	Line2f a; Rect2f b;
	a.m_PointA = Vector2f(-1.f, -1.f);
	a.m_PointB = Vector2f(+1.f, +1.f);
	b.m_Min = Vector2f(-1.f, 0.f);
	b.m_Max = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
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

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when both points of the Line are above the Segment.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, +5.f);
	a.m_PointB = Vector2f(+1.f, +5.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when both points of the Line are above the Segment, but Line direction is swapped.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(+1.f, +5.f);
	a.m_PointB = Vector2f(-1.f, +5.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when both points of the Line are above the Segment, but segment direction is swapped.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, +5.f);
	a.m_PointB = Vector2f(+1.f, +5.f);
	b.m_PointA = Vector2f(0.f, +1.f);
	b.m_PointB = Vector2f(-1.f, -1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Segment2f). Line and Segment don't overlap when both points of the Line are below the Segment.")
{
	Line2f a; Segment2f b;
	a.m_PointA = Vector2f(-1.f, -5.f);
	a.m_PointB = Vector2f(+1.f, -5.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Triangle2f). Line and Triangle overlap when Line points are either side of the Triangle.")
{
	Line2f a; Triangle2f b;
	a.m_PointA = Vector2f(-10.f, 0.f);
	a.m_PointB = Vector2f(+10.f, 0.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	b.m_PointC = Vector2f(+1.f, -1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Triangle2f). Line and Triangle overlap when first point of the Line is inside the Triangle.")
{
	Line2f a; Triangle2f b;
	a.m_PointA = Vector2f(0.f, 0.f);
	a.m_PointB = Vector2f(+10.f, 0.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	b.m_PointC = Vector2f(+1.f, -1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Triangle2f). Line and Triangle overlap when second point of the Line is inside the Triangle.")
{
	Line2f a; Triangle2f b;
	a.m_PointA = Vector2f(-10.f, 0.f);
	a.m_PointB = Vector2f(0.f, 0.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f( 0.f, +1.f);
	b.m_PointC = Vector2f(+1.f, -1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Triangle2f). Line and Triangle overlap when both points of the Line are inside the Triangle.")
{
	Line2f a; Triangle2f b;
	a.m_PointA = Vector2f(-1.f, -1.f);
	a.m_PointB = Vector2f(+1.f, -1.f);
	b.m_PointA = Vector2f(-2.f, -2.f);
	b.m_PointB = Vector2f(0.f, +2.f);
	b.m_PointC = Vector2f(+2.f, -2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Triangle2f). Line and Triangle don't overlap when both points of the Line are above the Triangle.")
{
	Line2f a; Triangle2f b;
	a.m_PointA = Vector2f(-1.f, +5.f);
	a.m_PointB = Vector2f(+1.f, +5.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	b.m_PointC = Vector2f(+1.f, -1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Line2f Triangle2f). Line and Triangle don't overlap when both points of the Line are below the Triangle.")
{
	Line2f a; Triangle2f b;
	a.m_PointA = Vector2f(-1.f, -5.f);
	a.m_PointB = Vector2f(+1.f, -5.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(0.f, +1.f);
	b.m_PointC = Vector2f(+1.f, -1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Circle2f).")
{
	// math::IsOverlapping(Circle2f Ray2f).
}

TEST_CASE("math::IsOverlapping(Ray2f Line2f).")
{
	// math::IsOverlapping(Line2f Ray2f).
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

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect overlap when ray runs through the centre of the rect.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-2.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect don't overlap when ray points in the wrong direction.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-2.f, 0.f);
	a.m_Direction = Vector2f(-1.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect overlap when ray runs through the centre of the rect away from origin.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-96.f, +97.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Min = Vector2f(+97.f, +96.f);
	b.m_Max = Vector2f(+98.f, +98.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect overlap when ray runs through the centre of the rect but points aren't either side.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-4.f, 0.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect don't overlap when ray misses the rect.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-2.f, -2.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect overlap when ray touches the edge of the rect.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-2.f, -1.f);
	a.m_Direction = Vector2f(+1.f, 0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Ray2f Rect2f). Ray and Rect overlap when ray touches the corner of the rect.")
{
	Ray2f a; Rect2f b;
	a.m_Position = Vector2f(-1.f, -1.f);
	a.m_Direction = Vector2f(+1.f, +1.f);
	b.m_Min = Vector2f(-1.f, 0.f);
	b.m_Max = Vector2f(0.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
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

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when rect min or rect max is inside the other rect.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(-2.f, -2.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+2.f, +2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects don't overlap when they are apart.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(-2.f, -2.f);
	a.m_Max = Vector2f(-1.f, -1.f);
	b.m_Min = Vector2f(+1.f, +1.f);
	b.m_Max = Vector2f(+2.f, +2.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when they share an edge.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+0.f, +0.f);
	b.m_Min = Vector2f(-1.f, +0.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when they share a corner.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+0.f, +0.f);
	b.m_Min = Vector2f(+0.f, +0.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when they are identical.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when one encloses the other.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_Min = Vector2f(-2.f, -2.f);
	b.m_Max = Vector2f(+2.f, +2.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when one is just a point inside the other.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(0.f);
	a.m_Max = Vector2f(0.f);
	b.m_Min = Vector2f(-1.f, -1.f);
	b.m_Max = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Rect2f). Rects overlap when they are identical and just a point.")
{
	Rect2f a, b;
	a.m_Min = Vector2f(0.f);
	a.m_Max = Vector2f(0.f);
	b.m_Min = Vector2f(0.f);
	b.m_Max = Vector2f(0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment overlap when segment runs through the centre of the rect.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_PointA = Vector2f(-2.f, 0.f);
	b.m_PointB = Vector2f(+2.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment overlap when segment runs through the centre of the rect away from origin.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(+97.f, +96.f);
	a.m_Max = Vector2f(+98.f, +98.f);
	b.m_PointA = Vector2f(-96.f, +97.f);
	b.m_PointB = Vector2f(+99.f, +97.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment overlap when segment runs through the centre of the rect but points aren't either side.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_PointA = Vector2f(-4.f, 0.f);
	b.m_PointB = Vector2f(-2.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment don't overlap when segment misses the rect.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_PointA = Vector2f(-2.f, -2.f);
	b.m_PointB = Vector2f(+2.f, -2.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment overlap when segment touches the edge of the rect.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b.m_PointA = Vector2f(-2.f, -1.f);
	b.m_PointB = Vector2f(+2.f, -1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment overlap when segment touches the corner of the rect.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(-1.f, 0.f);
	a.m_Max = Vector2f(0.f, +1.f);
	b.m_PointA = Vector2f(-1.f, -1.f);
	b.m_PointB = Vector2f(+1.f, +1.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Segment2f). Rect and Segment overlap when segment is contained inside the rect.")
{
	Rect2f a; Segment2f b;
	a.m_Min = Vector2f(-2.f, -2.f);
	a.m_Max = Vector2f(+2.f, +2.f);
	b.m_PointA = Vector2f(-1.f, 0.f);
	b.m_PointB = Vector2f(+1.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Vector2f). Rect and Point overlap when the point is enclosed by the rect.")
{
	Rect2f a; Vector2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b = Vector2f(0.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Vector2f). Rect and Point don't overlap when the point is outside the rect.")
{
	Rect2f a; Vector2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b = Vector2f(2.f, 0.f);
	CHECK(!math::IsOverlapping(a, b));
}

TEST_CASE("math::IsOverlapping(Rect2f Vector2f). Rect and Point overlap when the point touches the rect's edge.")
{
	Rect2f a; Vector2f b;
	a.m_Min = Vector2f(-1.f, -1.f);
	a.m_Max = Vector2f(+1.f, +1.f);
	b = Vector2f(1.f, 0.f);
	CHECK(math::IsOverlapping(a, b));
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

TEST_CASE("math::IsOverlapping(Vector2f Circle2f).")
{
	// math::IsOverlapping(Circle2f Vector2f).
}

TEST_CASE("math::IsOverlapping(Vector2f Rect2f).")
{
	// math::IsOverlapping(Rect2f Vector2f).
}