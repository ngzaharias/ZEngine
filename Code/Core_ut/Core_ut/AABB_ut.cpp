#include <Catch2/catch.hpp>

#include <Core/AABB.h>
#include <Core/Matrix.h>
#include <Core/OBB.h>
#include <Core/Rotator.h>
#include <Core/Vector.h>

TEST_CASE("AABB::Constructor")
{
	const AABB aabb = AABB(Vector3f::Zero, Vector3f::One);
	CHECK(aabb.m_Min == Vector3f::Zero);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB::Constructor with Negative")
{
	const AABB aabb = AABB(-Vector3f::One, Vector3f::One);
	CHECK(aabb.m_Min == -Vector3f::One);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB::FromExtents at Origin")
{
	const AABB aabb = AABB::FromExtents(Vector3f::Zero, Vector3f::One);
	CHECK(aabb.m_Min == -Vector3f::One);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB::FromExtents not at Origin")
{
	const AABB aabb = AABB::FromExtents(Vector3f::One, Vector3f::One);
	CHECK(aabb.m_Min == Vector3f::Zero);
	CHECK(aabb.m_Max == Vector3f(2.f));
}

TEST_CASE("AABB::FromOBB at Origin")
{
	const OBB obb = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
	const AABB aabb = AABB::FromOBB(obb);
	CHECK(aabb.m_Min == -Vector3f::One);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB::FromOBB not at Origin")
{
	const OBB obb = OBB::FromExtents(Vector3f::One, Matrix3x3::Identity, Vector3f::One);
	const AABB aabb = AABB::FromOBB(obb);
	CHECK(aabb.m_Min == Vector3f::Zero);
	CHECK(aabb.m_Max == Vector3f(2.f));
}

TEST_CASE("AABB::FromOBB with rotation on single axis.")
{
	const Rotator rotator = Rotator(0.f, 90.f, 0.f);
	const OBB obb = OBB::FromExtents(Vector3f::Zero, rotator, Vector3f::One);
	const AABB aabb = AABB::FromOBB(obb);
	CHECK(aabb.m_Min.x == Approx(-1.f));
	CHECK(aabb.m_Min.y == Approx(-1.f));
	CHECK(aabb.m_Min.z == Approx(-1.f));
	CHECK(aabb.m_Max.x == Approx(+1.f));
	CHECK(aabb.m_Max.y == Approx(+1.f));
	CHECK(aabb.m_Max.z == Approx(+1.f));
}

TEST_CASE("AABB::FromOBB with rotation on all axis.")
{
	const Rotator rotator = Rotator(90.f, 90.f, 90.f);
	const OBB obb = OBB::FromExtents(Vector3f::Zero, rotator, Vector3f::One);
	const AABB aabb = AABB::FromOBB(obb);
	CHECK(aabb.m_Min.x == Approx(-1.f));
	CHECK(aabb.m_Min.y == Approx(-1.f));
	CHECK(aabb.m_Min.z == Approx(-1.f));
	CHECK(aabb.m_Max.x == Approx(+1.f));
	CHECK(aabb.m_Max.y == Approx(+1.f));
	CHECK(aabb.m_Max.z == Approx(+1.f));
}
