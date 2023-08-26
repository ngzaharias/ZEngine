#include <Catch2/catch.hpp>

#include <Core/AABB.h>
#include <Core/Matrix.h>
#include <Core/OBB.h>
#include <Core/Rotator.h>
#include <Core/Vector.h>

TEST_CASE("AABB")
{
	{
		AABB aabb = AABB(Vector3f::Zero, Vector3f::One);
		CHECK(aabb.m_Min == Vector3f::Zero);
		CHECK(aabb.m_Max == Vector3f::One);
	}

	{
		AABB aabb = AABB(-Vector3f::One, Vector3f::One);
		CHECK(aabb.m_Min == -Vector3f::One);
		CHECK(aabb.m_Max ==  Vector3f::One);
	}

	{
		AABB aabb = AABB::FromExtents(Vector3f::Zero, Vector3f::One);
		CHECK(aabb.m_Min == -Vector3f::One);
		CHECK(aabb.m_Max == Vector3f::One);
	}

	{
		AABB aabb = AABB::FromExtents(Vector3f::One, Vector3f::One);
		CHECK(aabb.m_Min == Vector3f::Zero);
		CHECK(aabb.m_Max == Vector3f(2.f));
	}

	// Matrix
	{
		OBB obb = OBB::FromExtents(Vector3f::Zero, Matrix3x3::Identity, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min == -Vector3f::One);
		CHECK(aabb.m_Max == Vector3f::One);
	}

	// Matrix
	{
		OBB obb = OBB::FromExtents(Vector3f::One, Matrix3x3::Identity, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min == Vector3f::Zero);
		CHECK(aabb.m_Max == Vector3f(2.f));
	}

	// Matrix
	{
		Rotator rotator = Rotator(0.f, 90.f, 0.f);
		Matrix3x3 matrix = Matrix3x3::FromRotate(rotator);
		OBB obb = OBB::FromExtents(Vector3f::Zero, matrix, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min.x == Approx(-1.f));
		CHECK(aabb.m_Min.y == Approx(-1.f));
		CHECK(aabb.m_Min.z == Approx(-1.f));
		CHECK(aabb.m_Max.x == Approx(+1.f));
		CHECK(aabb.m_Max.y == Approx(+1.f));
		CHECK(aabb.m_Max.z == Approx(+1.f));
	}

	// Matrix
	{
		Rotator rotator = Rotator(90.f, 90.f, 90.f);
		Matrix3x3 matrix = Matrix3x3::FromRotate(rotator);
		OBB obb = OBB::FromExtents(Vector3f::Zero, matrix, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min.x == Approx(-1.f));
		CHECK(aabb.m_Min.y == Approx(-1.f));
		CHECK(aabb.m_Min.z == Approx(-1.f));
		CHECK(aabb.m_Max.x == Approx(+1.f));
		CHECK(aabb.m_Max.y == Approx(+1.f));
		CHECK(aabb.m_Max.z == Approx(+1.f));
	}

	// Quaternion
	{
		OBB obb = OBB::FromExtents(Vector3f::Zero, Quaternion::Identity, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min == -Vector3f::One);
		CHECK(aabb.m_Max == Vector3f::One);
	}

	// Quaternion
	{
		OBB obb = OBB::FromExtents(Vector3f::One, Quaternion::Identity, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min == Vector3f::Zero);
		CHECK(aabb.m_Max == Vector3f(2.f));
	}

	// Quaternion
	{
		Rotator rotator = Rotator(0.f, 90.f, 0.f);
		Quaternion quaternion = Quaternion::FromRotator(rotator);
		OBB obb = OBB::FromExtents(Vector3f::Zero, quaternion, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min.x == Approx(-1.f));
		CHECK(aabb.m_Min.y == Approx(-1.f));
		CHECK(aabb.m_Min.z == Approx(-1.f));
		CHECK(aabb.m_Max.x == Approx(+1.f));
		CHECK(aabb.m_Max.y == Approx(+1.f));
		CHECK(aabb.m_Max.z == Approx(+1.f));
	}

	// Quaternion
	{
		Rotator rotator = Rotator(90.f, 90.f, 90.f);
		Quaternion quaternion = Quaternion::FromRotator(rotator);
		OBB obb = OBB::FromExtents(Vector3f::Zero, quaternion, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min.x == Approx(-1.f));
		CHECK(aabb.m_Min.y == Approx(-1.f));
		CHECK(aabb.m_Min.z == Approx(-1.f));
		CHECK(aabb.m_Max.x == Approx(+1.f));
		CHECK(aabb.m_Max.y == Approx(+1.f));
		CHECK(aabb.m_Max.z == Approx(+1.f));
	}

	// Rotator
	{
		OBB obb = OBB::FromExtents(Vector3f::Zero, Rotator::Zero, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min == -Vector3f::One);
		CHECK(aabb.m_Max == Vector3f::One);
	}

	// Rotator
	{
		OBB obb = OBB::FromExtents(Vector3f::One, Rotator::Zero, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min == Vector3f::Zero);
		CHECK(aabb.m_Max == Vector3f(2.f));
	}

	// Rotator
	{
		Rotator rotator = Rotator(0.f, 90.f, 0.f);
		OBB obb = OBB::FromExtents(Vector3f::Zero, rotator, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min.x == Approx(-1.f));
		CHECK(aabb.m_Min.y == Approx(-1.f));
		CHECK(aabb.m_Min.z == Approx(-1.f));
		CHECK(aabb.m_Max.x == Approx(+1.f));
		CHECK(aabb.m_Max.y == Approx(+1.f));
		CHECK(aabb.m_Max.z == Approx(+1.f));
	}

	// Rotator
	{
		Rotator rotator = Rotator(90.f, 90.f, 90.f);
		OBB obb = OBB::FromExtents(Vector3f::Zero, rotator, Vector3f::One);
		AABB aabb = AABB::FromOBB(obb);
		CHECK(aabb.m_Min.x == Approx(-1.f));
		CHECK(aabb.m_Min.y == Approx(-1.f));
		CHECK(aabb.m_Min.z == Approx(-1.f));
		CHECK(aabb.m_Max.x == Approx(+1.f));
		CHECK(aabb.m_Max.y == Approx(+1.f));
		CHECK(aabb.m_Max.z == Approx(+1.f));
	}
}