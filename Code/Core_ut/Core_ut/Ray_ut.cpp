#include <Catch2/catch.hpp>

#include <Core/Ray.h>
#include <Core/Vector.h>

TEST_CASE("Ray")
{
	{
		Ray ray = Ray(Vector3f::Zero, Vector3f::AxisX);
		CHECK(ray.m_OriginPos == Vector3f::Zero);
		CHECK(ray.m_Direction == Vector3f::AxisX);
	}

	{
		Ray ray = Ray(Vector3f::One, Vector3f::AxisX);
		CHECK(ray.m_OriginPos == Vector3f::One);
		CHECK(ray.m_Direction == Vector3f::AxisX);
	}

	{
		Ray ray = Ray::FromPoints(Vector3f::One, Vector3f(2.f, 1.f, 1.f));
		CHECK(ray.m_OriginPos == Vector3f::One);
		CHECK(ray.m_Direction == Vector3f::AxisX);
	}

	{
		Ray ray = Ray::FromPoints(Vector3f::One, Vector3f(0.f, 1.f, 1.f));
		CHECK(ray.m_OriginPos == Vector3f::One);
		CHECK(ray.m_Direction == -Vector3f::AxisX);
	}
}