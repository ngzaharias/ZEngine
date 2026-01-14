#include "CorePCH.h"

#include "Math/Ray.h"
#include "Math/Vector.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Ray3f. " name, "[Ray3f]")

CLASS_TEST_CASE("Constructor.")
{
	{
		Ray3f ray = Ray3f(Vector3f::Zero, Vector3f::AxisX);
		CHECK(ray.m_Position == Vector3f::Zero);
		CHECK(ray.m_Direction == Vector3f::AxisX);
	}

	{
		Ray3f ray = Ray3f(Vector3f::One, Vector3f::AxisX);
		CHECK(ray.m_Position == Vector3f::One);
		CHECK(ray.m_Direction == Vector3f::AxisX);
	}
}

CLASS_TEST_CASE("FromPoints.")
{
	{
		Ray3f ray = Ray3f::FromPoints(Vector3f::One, Vector3f(2.f, 1.f, 1.f));
		CHECK(ray.m_Position == Vector3f::One);
		CHECK(ray.m_Direction == Vector3f::AxisX);
	}

	{
		Ray3f ray = Ray3f::FromPoints(Vector3f::One, Vector3f(0.f, 1.f, 1.f));
		CHECK(ray.m_Position == Vector3f::One);
		CHECK(ray.m_Direction == -Vector3f::AxisX);
	}
}