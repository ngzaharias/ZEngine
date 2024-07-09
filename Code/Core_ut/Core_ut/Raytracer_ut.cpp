#include <Catch2/catch.hpp>

#include "Core/Array.h"
#include "Core/Raytracer.h"
#include "Core/Vector.h"

TEST_CASE("path::Raytracer")
{
	SECTION("Iterator")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 1.f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisX * 4.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].x == 0);
		CHECK(values[1].x == 1);
		CHECK(values[2].x == 2);
		CHECK(values[3].x == 3);
		CHECK(values[4].x == 4);
	}

	SECTION("Size: 0.5f")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisX * 2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].x == 0);
		CHECK(values[1].x == 1);
		CHECK(values[2].x == 2);
		CHECK(values[3].x == 3);
		CHECK(values[4].x == 4);
	}

	SECTION("Size: 1.f")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 1.f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisX * 4.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].x == 0);
		CHECK(values[1].x == 1);
		CHECK(values[2].x == 2);
		CHECK(values[3].x == 3);
		CHECK(values[4].x == 4);
	}

	SECTION("Size: 2.f")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 2.f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisX * 8.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].x == 0);
		CHECK(values[1].x == 1);
		CHECK(values[2].x == 2);
		CHECK(values[3].x == 3);
		CHECK(values[4].x == 4);
	}

	SECTION("-X-Axis")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisX * -2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].x == 0);
		CHECK(values[1].x == -1);
		CHECK(values[2].x == -2);
		CHECK(values[3].x == -3);
		CHECK(values[4].x == -4);
	}

	SECTION("+X-Axis")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisX * 2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].x == 0);
		CHECK(values[1].x == 1);
		CHECK(values[2].x == 2);
		CHECK(values[3].x == 3);
		CHECK(values[4].x == 4);
	}

	SECTION("-Y-Axis")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisY * -2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].y == 0);
		CHECK(values[1].y == -1);
		CHECK(values[2].y == -2);
		CHECK(values[3].y == -3);
		CHECK(values[4].y == -4);
	}

	SECTION("+Y-Axis")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisY * 2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].y == 0);
		CHECK(values[1].y == 1);
		CHECK(values[2].y == 2);
		CHECK(values[3].y == 3);
		CHECK(values[4].y == 4);
	}

	SECTION("-Z-Axis")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisZ * -2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].z == 0);
		CHECK(values[1].z == -1);
		CHECK(values[2].z == -2);
		CHECK(values[3].z == -3);
		CHECK(values[4].z == -4);
	}

	SECTION("+Z-Axis")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::AxisZ * 2.f;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 5);
		CHECK(values[0].z == 0);
		CHECK(values[1].z == 1);
		CHECK(values[2].z == 2);
		CHECK(values[3].z == 3);
		CHECK(values[4].z == 4);
	}

	SECTION("XYZ")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 0.5f;
		tracer.m_Source = Vector3f::Zero;
		tracer.m_Target = Vector3f::One;

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 7);
		CHECK(values[0] == Vector3i(0, 0, 0));
		CHECK(values[1] == Vector3i(0, 0, 1));
		CHECK(values[2] == Vector3i(0, 1, 1));
		CHECK(values[3] == Vector3i(1, 1, 1));
		CHECK(values[4] == Vector3i(1, 1, 2));
		CHECK(values[5] == Vector3i(1, 2, 2));
		CHECK(values[6] == Vector3i(2, 2, 2));
	}

	SECTION("Real World Example")
	{
		path::Raytracer tracer;
		tracer.m_CellSize = 2.f;
		tracer.m_Source = Vector3f(-123.45f, 543.21f, 321.54f);
		tracer.m_Target = tracer.m_Source + Vector3f(3.33f, -7.77f, 1.11f);

		Array<Vector3i> values;
		for (const Vector3i& value : tracer)
			values.Append(value);

		REQUIRE(values.GetCount() == 7);
		CHECK(values[0] == Vector3i(-62, 271, 160));
		CHECK(values[1] == Vector3i(-62, 270, 160));
		CHECK(values[2] == Vector3i(-62, 269, 160));
		CHECK(values[3] == Vector3i(-62, 269, 161));
		CHECK(values[4] == Vector3i(-61, 269, 161));
		CHECK(values[5] == Vector3i(-61, 268, 161));
		CHECK(values[6] == Vector3i(-61, 267, 161));
	}
}
