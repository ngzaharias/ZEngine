#include <Catch2/catch.hpp>

#include <Core/AABB.h>
#include <Core/CollisionMath.h>
#include <Core/Line.h>
#include <Core/OBB.h>
#include <Core/Plane.h>
#include <Core/Ray.h>
#include <Core/Sphere.h>

TEST_CASE("math::Intersection(Line, Plane)")
{
	Line a;
	Plane b;
	Vector3f c;

	{
		INFO("No Collision.");
		INFO("Plane Normal and Line are perpendicular to each other.");
		a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
		a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
		b.m_Normal = Vector3f(0.f, 1.f, 0.f);
		b.m_Point = Vector3f(0.f, 0.f, 0.f);

		CHECK(!math::Intersection(a, b, c));
	}

	{
		INFO("No Collision.");
		INFO("Line stops on the plane.");
		a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
		a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
		b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
		b.m_Point = Vector3f(+1.f, 0.f, 0.f);

		CHECK(math::Intersection(a, b, c));
		CHECK(c == Vector3f(+1.f, 0.f, 0.f));
	}

	{
		INFO("No Collision.");
		INFO("Line stops short of the plane.");
		a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
		a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
		b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
		b.m_Point = Vector3f(+1.01f, 0.f, 0.f);

		CHECK(!math::Intersection(a, b, c));
	}

	{
		INFO("Collision.");
		a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
		a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
		b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
		b.m_Point = Vector3f(0.f, 0.f, 0.f);

		CHECK(math::Intersection(a, b, c));
		CHECK(c == Vector3f::Zero);
	}
}

TEST_CASE("math::Intersection(Ray, Plane)")
{
	Ray a;
	Plane b;
	Vector3f c;

	{
		INFO("No Collision.");
		INFO("Plane Normal and Ray are perpendicular to each other.");
		a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
		a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
		b.m_Normal = Vector3f(0.f, 1.f, 0.f);
		b.m_Point = Vector3f(0.f, 0.f, 0.f);

		CHECK(!math::Intersection(a, b, c));
	}

	{
		INFO("Collision.");
		a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
		a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
		b.m_Normal = Vector3f(+1.f, 0.f, 0.f);
		b.m_Point = Vector3f(0.f, 0.f, 0.f);

		CHECK(math::Intersection(a, b, c));
		CHECK(c == Vector3f::Zero);
	}
}

TEST_CASE("math::IntersectionXZ(Line, Line)")
{
	Line a, b;
	Vector3f c;

	{
		INFO("Collision.");
		a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
		a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
		b.m_PointA = Vector3f(0.f, 0.f, -1.f);
		b.m_PointB = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IntersectionXZ(a, b, c));
		CHECK(c == Vector3f::Zero);
	}

	{
		INFO("No Collision.");
		a.m_PointA = Vector3f(-1.f, 0.f, -1.f);
		a.m_PointB = Vector3f(+1.f, 0.f, -1.f);
		b.m_PointA = Vector3f(-1.f, 0.f, +1.f);
		b.m_PointB = Vector3f(+1.f, 0.f, +1.f);

		CHECK(!math::IntersectionXZ(a, b, c));
	}
}

TEST_CASE("math::IntersectionXZ(Ray, Ray)")
{
	Ray a, b;
	Vector3f c;

	{
		INFO("Collision.");
		a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
		a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
		b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IntersectionXZ(a, b, c));
		CHECK(c == Vector3f::Zero);
	}

	{
		INFO("Collision.");
		a.m_OriginPos = Vector3f(-10.f, 0.f, 0.f);
		a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
		b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IntersectionXZ(a, b, c));
		CHECK(c == Vector3f::Zero);
	}

	{
		INFO("Collision.");
		a.m_OriginPos = Vector3f(-1.f, -1.f, 0.f);
		a.m_Direction = Vector3f(+1.f, -1.f, 0.f);
		b.m_OriginPos = Vector3f(0.f, +1.f, -1.f);
		b.m_Direction = Vector3f(0.f, +1.f, +1.f);

		CHECK(math::IntersectionXZ(a, b, c));
		CHECK(c == Vector3f::Zero);
	}

	{
		INFO("Parallel Lines.");
		a.m_OriginPos = Vector3f(-1.f, 0.f, -1.f);
		a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
		b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
		b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

		CHECK(!math::IntersectionXZ(a, b, c));
	}

	{
		INFO("Parallel Lines With Further Origin.");
		a.m_OriginPos = Vector3f(-10.f, 0.f, -1.f);
		a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
		b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
		b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

		CHECK(!math::IntersectionXZ(a, b, c));
	}
}

TEST_CASE("math::IsAxisIntersecting(OBB, OBB, Axis)")
{
}

TEST_CASE("math::IsOverlapping(AABB, AABB)")
{
	AABB a, b;

	{
		INFO("Collision.");
		a.m_Min = Vector3f(0.0f);
		a.m_Max = Vector3f(1.0f);
		b.m_Min = Vector3f(0.5f);
		b.m_Max = Vector3f(1.5f);
		CHECK(math::IsOverlapping(a, b));
	}

	{
		INFO("No Collision.");
		a.m_Min = Vector3f(0.f);
		a.m_Max = Vector3f(1.f);
		b.m_Min = Vector3f(2.f);
		b.m_Max = Vector3f(3.f);
		CHECK(!math::IsOverlapping(a, b));
	}

	{
		INFO("Share Corner.");
		a.m_Min = Vector3f(0.f);
		a.m_Max = Vector3f(1.f);
		b.m_Min = Vector3f(1.f);
		b.m_Max = Vector3f(2.f);
		CHECK(!math::IsOverlapping(a, b));
	}

	{
		INFO("Share Edge.");
		a.m_Min = Vector3f(0.f);
		a.m_Max = Vector3f(1.f);
		b.m_Min = Vector3f(1.f, 0.f, 0.f);
		b.m_Max = Vector3f(1.f, 2.f, 2.f);
		CHECK(!math::IsOverlapping(a, b));
	}

	{
		INFO("A Inside B.");
		a.m_Min = Vector3f(-1.f);
		a.m_Max = Vector3f(+1.f);
		b.m_Min = Vector3f(-2.f);
		b.m_Max = Vector3f(+2.f);
		CHECK(math::IsOverlapping(a, b));
	}

	{
		INFO("B Inside A.");
		a.m_Min = Vector3f(-2.f);
		a.m_Max = Vector3f(+2.f);
		b.m_Min = Vector3f(-1.f);
		b.m_Max = Vector3f(+1.f);
		CHECK(math::IsOverlapping(a, b));
	}
}

TEST_CASE("math::IsOverlapping(AABB, Ray)")
{
	AABB aabb;
	aabb.m_Min = Vector3f::Zero;
	aabb.m_Max = Vector3f::One;

	Ray ray;

	{
		INFO("Collision.");
		ray.m_Direction = -Vector3f::AxisX;
		ray.m_OriginPos = Vector3f(1.5f, 0.5f, 0.5f);
		CHECK(math::IsOverlapping(aabb, ray));
	}

	{
		INFO("No Collision.");
		ray.m_Direction = Vector3f::AxisX;
		ray.m_OriginPos = Vector3f(1.5f, 0.5f, 0.5f);
		CHECK(!math::IsOverlapping(aabb, ray));
	}

	{
		INFO("Glancing Corner.");
		ray.m_Direction = Vector3f::AxisZ;
		ray.m_OriginPos = Vector3f(1.f);
		CHECK(!math::IsOverlapping(aabb, ray));
	}

	{
		INFO("Glancing Edge.");
		ray.m_Direction = Vector3f::AxisZ;
		ray.m_OriginPos = Vector3f(1.f, 0.5f, -1.f);
		CHECK(!math::IsOverlapping(aabb, ray));
	}
}

TEST_CASE("math::IsOverlapping(OBB, OBB)")
{
	constexpr Vector3f s_Extents = Vector3f(0.5f);

	OBB a, b;

	{
		INFO("Collision, No Rotation.");
		a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_Extents);
		b = OBB::FromExtents(Vector3f(1.0f), Matrix3x3::Identity, s_Extents);
		CHECK(math::IsOverlapping(a, b));
	}

	{
		INFO("No Collision, No Rotation.");
		a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_Extents);
		b = OBB::FromExtents(Vector3f(2.5f), Matrix3x3::Identity, s_Extents);
		CHECK(!math::IsOverlapping(a, b));
	}

	{
		INFO("Share Corner, No Rotation.");
		a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_Extents);
		b = OBB::FromExtents(Vector3f(1.5f), Matrix3x3::Identity, s_Extents);
		CHECK(math::IsOverlapping(a, b));
	}

	{
		INFO("Share Edge, No Rotation.");
		a = OBB::FromExtents(Vector3f(0.5f), Matrix3x3::Identity, s_Extents);
		b = OBB::FromExtents(Vector3f(1.5f, 0.f, 0.f), Matrix3x3::Identity, s_Extents);
		CHECK(math::IsOverlapping(a, b));
	}

	{
		INFO("A Inside B, No Rotation.");
		a = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
		b = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f(2.f));
		CHECK(math::IsOverlapping(a, b));
	}

	{
		INFO("B Inside A, No Rotation.");
		a = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f(2.f));
		b = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
		CHECK(math::IsOverlapping(a, b));
	}
}

TEST_CASE("math::IsOverlappingXZ(Circle, Circle)")
{
	Sphere a, b;

	{
		INFO("Collision.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_Position = Vector3f(1.f, 0.f, 0.f);
		b.m_Radius = 1.f;

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("No Collision.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_Position = Vector3f(3.f, 0.f, 0.f);
		b.m_Radius = 1.f;

		CHECK(!math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision with 0 Radius.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_Position = Vector3f(0.f, 0.f, 0.f);
		b.m_Radius = 0.f;

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision with Touching Circles.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_Position = Vector3f(2.f, 0.f, 0.f);
		b.m_Radius = 1.f;

		CHECK(!math::IsOverlappingXZ(a, b));
	}
}

TEST_CASE("math::IsOverlappingXZ(Circle, Line)")
{
	Sphere a; Line b;

	{
		INFO("Collision through Origin.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_PointA = Vector3f(0.f, 0.f, -2.f);
		b.m_PointB = Vector3f(0.f, 0.f, +2.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("No Collision.");
		a.m_Position = Vector3f(2.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_PointA = Vector3f(0.f, 0.f, -1.f);
		b.m_PointB = Vector3f(0.f, 0.f, +1.f);

		CHECK(!math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision on Circle's Edge.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_PointA = Vector3f(1.f, 0.f, -1.f);
		b.m_PointB = Vector3f(1.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}
}

TEST_CASE("math::IsOverlappingXZ(Circle, Ray)")
{
	Sphere a; Ray b;

	{
		INFO("Collision through Origin.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision through Origin further away.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_OriginPos = Vector3f(0.f, 0.f, -10.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("No Collision.");
		a.m_Position = Vector3f(2.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(!math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision on Circle's Edge.");
		a.m_Position = Vector3f(0.f, 0.f, 0.f);
		a.m_Radius = 1.f;
		b.m_OriginPos = Vector3f(1.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}
}

TEST_CASE("math::IsOverlappingXZ(Line, Line)")
{
	Line a, b;

	{
		INFO("Collision.");
		a.m_PointA = Vector3f(-1.f, 0.f, 0.f);
		a.m_PointB = Vector3f(+1.f, 0.f, 0.f);
		b.m_PointA = Vector3f(0.f, 0.f, -1.f);
		b.m_PointB = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("No Collision.");
		a.m_PointA = Vector3f(-10.f, 0.f, 0.f);
		a.m_PointB = Vector3f(-0.1f, 0.f, 0.f);
		b.m_PointA = Vector3f(0.f, 0.f, -1.f);
		b.m_PointB = Vector3f(0.f, 0.f, +1.f);

		CHECK(!math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision.");
		a.m_PointA = Vector3f(-1.f, -1.f, 0.f);
		a.m_PointB = Vector3f(+1.f, -1.f, 0.f);
		b.m_PointA = Vector3f(0.f, +1.f, -1.f);
		b.m_PointB = Vector3f(0.f, +1.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Parallel Lines.");
		a.m_PointA = Vector3f(-1.f, 0.f, -1.f);
		a.m_PointB = Vector3f(+1.f, 0.f, -1.f);
		b.m_PointA = Vector3f(-1.f, 0.f, +1.f);
		b.m_PointB = Vector3f(+1.f, 0.f, +1.f);

		CHECK(!math::IsOverlappingXZ(a, b));
	}
}

TEST_CASE("math::IsOverlappingXZ(Ray, Ray)")
{
	Ray a, b;

	{
		INFO("Collision.");
		a.m_OriginPos = Vector3f(-1.f, 0.f, 0.f);
		a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
		b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision.");
		a.m_OriginPos = Vector3f(-10.f, 0.f, 0.f);
		a.m_Direction = Vector3f(+1.f, 0.f, 0.f);
		b.m_OriginPos = Vector3f(0.f, 0.f, -1.f);
		b.m_Direction = Vector3f(0.f, 0.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Collision.");
		a.m_OriginPos = Vector3f(-1.f, -1.f, 0.f);
		a.m_Direction = Vector3f(+1.f, -1.f, 0.f);
		b.m_OriginPos = Vector3f(0.f, +1.f, -1.f);
		b.m_Direction = Vector3f(0.f, +1.f, +1.f);

		CHECK(math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Parallel Lines.");
		a.m_OriginPos = Vector3f(-1.f, 0.f, -1.f);
		a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
		b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
		b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

		CHECK(!math::IsOverlappingXZ(a, b));
	}

	{
		INFO("Parallel Lines With Further Origin.");
		a.m_OriginPos = Vector3f(-10.f, 0.f, -1.f);
		a.m_Direction = Vector3f(+1.f, 0.f, -1.f);
		b.m_OriginPos = Vector3f(-1.f, 0.f, +1.f);
		b.m_Direction = Vector3f(+1.f, 0.f, +1.f);

		CHECK(!math::IsOverlappingXZ(a, b));
	}
}
