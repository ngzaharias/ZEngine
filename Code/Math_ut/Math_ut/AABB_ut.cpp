#include "MathPCH.h"

#include "Math/AABB.h"
#include "Math/Matrix.h"
#include "Math/OBB.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

TEST_CASE("AABB3f. Constructor.")
{
	const AABB3f aabb = AABB3f(Vector3f::Zero, Vector3f::One);
	CHECK(aabb.m_Min == Vector3f::Zero);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB3f. Constructor with Negative.")
{
	const AABB3f aabb = AABB3f(-Vector3f::One, Vector3f::One);
	CHECK(aabb.m_Min == -Vector3f::One);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB3f. FromExtents at Origin.")
{
	const AABB3f aabb = AABB3f::FromExtents(Vector3f::Zero, Vector3f::One);
	CHECK(aabb.m_Min == -Vector3f::One);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB3f. FromExtents not at Origin.")
{
	const AABB3f aabb = AABB3f::FromExtents(Vector3f::One, Vector3f::One);
	CHECK(aabb.m_Min == Vector3f::Zero);
	CHECK(aabb.m_Max == Vector3f(2.f));
}

TEST_CASE("AABB3f. FromOBB at Origin.")
{
	const OBB3f obb = OBB3f::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
	const AABB3f aabb = AABB3f::FromOBB(obb);
	CHECK(aabb.m_Min == -Vector3f::One);
	CHECK(aabb.m_Max == Vector3f::One);
}

TEST_CASE("AABB3f. FromOBB not at Origin.")
{
	const OBB3f obb = OBB3f::FromExtents(Vector3f::One, Matrix3x3::Identity, Vector3f::One);
	const AABB3f aabb = AABB3f::FromOBB(obb);
	CHECK(aabb.m_Min == Vector3f::Zero);
	CHECK(aabb.m_Max == Vector3f(2.f));
}

TEST_CASE("AABB3f. FromOBB with rotation on single axis..")
{
	const Rotator rotator = Rotator(0.f, 90.f, 0.f);
	const OBB3f obb = OBB3f::FromExtents(Vector3f::Zero, rotator, Vector3f::One);
	const AABB3f aabb = AABB3f::FromOBB(obb);
	CHECK(aabb.m_Min.x == Approx(-1.f));
	CHECK(aabb.m_Min.y == Approx(-1.f));
	CHECK(aabb.m_Min.z == Approx(-1.f));
	CHECK(aabb.m_Max.x == Approx(+1.f));
	CHECK(aabb.m_Max.y == Approx(+1.f));
	CHECK(aabb.m_Max.z == Approx(+1.f));
}

TEST_CASE("AABB3f. FromOBB with rotation on all axis..")
{
	const Rotator rotator = Rotator(90.f, 90.f, 90.f);
	const OBB3f obb = OBB3f::FromExtents(Vector3f::Zero, rotator, Vector3f::One);
	const AABB3f aabb = AABB3f::FromOBB(obb);
	CHECK(aabb.m_Min.x == Approx(-1.f));
	CHECK(aabb.m_Min.y == Approx(-1.f));
	CHECK(aabb.m_Min.z == Approx(-1.f));
	CHECK(aabb.m_Max.x == Approx(+1.f));
	CHECK(aabb.m_Max.y == Approx(+1.f));
	CHECK(aabb.m_Max.z == Approx(+1.f));
}
