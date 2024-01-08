#include <Catch2/catch.hpp>

#include <Core/Common.h>
#include <Core/VectorMath.h>

TEST_CASE("math::Vector2f::IsNearly")
{
	{
		INFO("Exact: Near Origin.");

		const Vector2f a = Vector2f(1.23f, 4.56f);
		const Vector2f b = Vector2f(1.23f, 4.56f);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Exact: Far from Origin.");

		const Vector2f a = Vector2f(KINDA_LARGE_FLOAT);
		const Vector2f b = Vector2f(KINDA_LARGE_FLOAT);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Nearly: Near Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.13f;
		const Vector2f a = Vector2f(1.23f + offsetA, 4.56f);
		const Vector2f b = Vector2f(1.23f + offsetB, 4.56f);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Nearly: Far from Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.4f;
		const Vector2f a = Vector2f(KINDA_LARGE_FLOAT + offsetA, KINDA_LARGE_FLOAT);
		const Vector2f b = Vector2f(KINDA_LARGE_FLOAT + offsetB, KINDA_LARGE_FLOAT);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Not Nearly: Near Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.14f;
		const Vector2f a = Vector2f(1.23f + offsetA, 4.56f);
		const Vector2f b = Vector2f(1.23f + offsetB, 4.56f);
		CHECK(!IsNearly(a, b));
	}

	{
		INFO("Not Nearly: Far from Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.5f;
		const Vector2f a = Vector2f(KINDA_LARGE_FLOAT + offsetA, KINDA_LARGE_FLOAT);
		const Vector2f b = Vector2f(KINDA_LARGE_FLOAT + offsetB, KINDA_LARGE_FLOAT);
		CHECK(!IsNearly(a, b));
	}
}

TEST_CASE("math::Vector3f::IsNearly")
{
	{
		INFO("Exact: Near Origin.");

		const Vector3f a = Vector3f(1.23f, 4.56f, 7.89f);
		const Vector3f b = Vector3f(1.23f, 4.56f, 7.89f);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Exact: Far from Origin.");

		const Vector3f a = Vector3f(KINDA_LARGE_FLOAT);
		const Vector3f b = Vector3f(KINDA_LARGE_FLOAT);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Nearly: Near Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.13f;
		const Vector3f a = Vector3f(1.23f + offsetA, 4.56f, 7.89f);
		const Vector3f b = Vector3f(1.23f + offsetB, 4.56f, 7.89f);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Nearly: Far from Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.4f;
		const Vector3f a = Vector3f(KINDA_LARGE_FLOAT + offsetA, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
		const Vector3f b = Vector3f(KINDA_LARGE_FLOAT + offsetB, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
		CHECK(IsNearly(a, b));
	}

	{
		INFO("Not Nearly: Near Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.14f;
		const Vector3f a = Vector3f(1.23f + offsetA, 4.56f, 7.89f);
		const Vector3f b = Vector3f(1.23f + offsetB, 4.56f, 7.89f);
		CHECK(!IsNearly(a, b));
	}

	{
		INFO("Not Nearly: Far from Origin.");

		const float offsetA = 0.1f;
		const float offsetB = 0.5f;
		const Vector3f a = Vector3f(KINDA_LARGE_FLOAT + offsetA, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
		const Vector3f b = Vector3f(KINDA_LARGE_FLOAT + offsetB, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
		CHECK(!IsNearly(a, b));
	}
}

TEST_CASE("math::Vector2f::Angle")
{
	{
		INFO("Parallel: Same.");
		const Vector2f a = Vector2f(0.f, 1.f);
		const Vector2f b = Vector2f(0.f, 1.f);
		const float angle = math::Angle(a, b);
		CHECK(angle == 0.f);
	}

	{
		INFO("Perpendicular.");
		const Vector2f a = Vector2f(0.f, 1.f);
		const Vector2f b = Vector2f(1.f, 0.f);
		const float angle = math::Angle(a, b);
		CHECK(angle == PI_HALF);
	}

	{
		INFO("Parallel: Opposite.");
		const Vector2f a = Vector2f(0.f, +1.f);
		const Vector2f b = Vector2f(0.f, -1.f);
		const float angle = math::Angle(a, b);
		CHECK(angle == Approx(PI_ONE));
	}
}

TEST_CASE("math::Vector3f::Angle")
{
	{
		INFO("Parallel: Same.");
		const Vector3f a = Vector3f(0.f, 0.f, 1.f);
		const Vector3f b = Vector3f(0.f, 0.f, 1.f);
		const float angle = math::Angle(a, b);
		CHECK(angle == 0.f);
	}

	{
		INFO("Perpendicular.");
		const Vector3f a = Vector3f(0.f, 0.f, 1.f);
		const Vector3f b = Vector3f(1.f, 0.f, 0.f);
		const float angle = math::Angle(a, b);
		CHECK(angle == PI_HALF);
	}

	{
		INFO("Parallel: Opposite.");
		const Vector3f a = Vector3f(0.f, 0.f, +1.f);
		const Vector3f b = Vector3f(0.f, 0.f, -1.f);
		const float angle = math::Angle(a, b);
		CHECK(angle == Approx(PI_ONE));
	}
}

TEST_CASE("math::Vector2f::Clamp")
{
	const Vector2f min = Vector2f(1.f, 2.f);
	const Vector2f max = Vector2f(5.f, 6.f);

	// nothing clamped
	CHECK(math::Clamp(Vector2f(3.f, 4.f), min, max) == Vector2f(3.f, 4.f));

	// x and y clamped to max
	CHECK(math::Clamp(Vector2f(+10.f, +20.f), min, max) == max);
	// x only clamped to max
	CHECK(math::Clamp(Vector2f(10.f, 4.f), min, max) == Vector2f(max.x, 4.f));
	// y only clamped to max
	CHECK(math::Clamp(Vector2f(3.f, 20.f), min, max) == Vector2f(3.f, max.y));

	// x and y clamped to min
	CHECK(math::Clamp(Vector2f(-10.f, -20.f), min, max) == min);
	// x only clamped to min
	CHECK(math::Clamp(Vector2f(-10.f, 4.f), min, max) == Vector2f(min.x, 4.f));
	// y only clamped to min
	CHECK(math::Clamp(Vector2f(3.f, -20.f), min, max) == Vector2f(3.f, min.y));
}

TEST_CASE("math::Vector3f::Cross")
{
	CHECK(math::Cross(Vector3f::AxisX, Vector3f::AxisY) == Vector3f::AxisZ);
	CHECK(math::Cross(Vector3f::AxisY, Vector3f::AxisZ) == Vector3f::AxisX);
	CHECK(math::Cross(Vector3f::AxisZ, Vector3f::AxisX) == Vector3f::AxisY);
}

TEST_CASE("math::Vector2f::Distance")
{
}

TEST_CASE("math::Vector2f::DistanceSqr")
{
}

TEST_CASE("math::Vector2f::Dot")
{
}

TEST_CASE("math::Vector3f::Dot")
{
	CHECK(math::Dot(Vector3f::AxisX, Vector3f::AxisX) == 1.f);
	CHECK(math::Dot(Vector3f::AxisX, Vector3f::AxisY) == 0.f);
	CHECK(math::Dot(Vector3f::AxisX, Vector3f::AxisZ) == 0.f);
	CHECK(math::Dot(Vector3f::AxisX, -Vector3f::AxisX) == -1.f);
}

TEST_CASE("math::Vector2f::Divide")
{
}

TEST_CASE("math::Vector2f::Max")
{
}

TEST_CASE("math::Vector2f::Min")
{
}

TEST_CASE("math::Vector2f::Multiply")
{
}

TEST_CASE("math::Vector2f::Perpendicular")
{
}

TEST_CASE("math::Vector3f::ProjectOntoLine(Vector3f)")
{
}

TEST_CASE("math::Vector3f::ProjectOntoLineClamped(Vector3f)")
{
}

TEST_CASE("math::Vector3f::ProjectOntoLineXZ(Vector3f)")
{
}

TEST_CASE("math::Vector3f::ProjectOntoLineXZClamped(Vector3f)")
{
}

TEST_CASE("math::Vector3f::ProjectOntoRay(Vector3f)")
{
}

TEST_CASE("math::Vector3f::ProjectOntoRayXZ(Vector3f)")
{
}

TEST_CASE("math::Vector2f::Reflect")
{
}

TEST_CASE("math::Vector2f::ToGridPos")
{
	{
		INFO("Origin.");

		const Vector2f input = Vector2f::Zero;
		const Vector2i outputA = math::ToGridPos(input, 0.1f);
		const Vector2i outputB = math::ToGridPos(input, 1.0f);
		const Vector2i outputC = math::ToGridPos(input, 10.f);
		CHECK(outputA == Vector2i::Zero);
		CHECK(outputB == Vector2i::Zero);
		CHECK(outputC == Vector2i::Zero);
	}

	{
		INFO("Inside: Centre.");

		const Vector2f inputA = Vector2f(00.15f);
		const Vector2f inputB = Vector2f(01.5f);
		const Vector2f inputC = Vector2f(15.f);
		const Vector2i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector2i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector2i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA == Vector2i(1));
		CHECK(outputB == Vector2i(1));
		CHECK(outputC == Vector2i(1));
	}

	{
		INFO("Inside: Bottom Left.");

		const Vector2f inputA = Vector2f(.1f);
		const Vector2f inputB = Vector2f(1.0f);
		const Vector2f inputC = Vector2f(10.0f);
		const Vector2i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector2i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector2i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA == Vector2i(1));
		CHECK(outputB == Vector2i(1));
		CHECK(outputC == Vector2i(1));
	}

	{
		INFO("Inside: Top Right.");

		const Vector2f inputA = Vector2f(00.199f);
		const Vector2f inputB = Vector2f(01.99f);
		const Vector2f inputC = Vector2f(19.9f);
		const Vector2i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector2i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector2i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA == Vector2i(1));
		CHECK(outputB == Vector2i(1));
		CHECK(outputC == Vector2i(1));
	}

	{
		INFO("Outside: Bottom Left.");

		const Vector2f inputA = Vector2f(.099f);
		const Vector2f inputB = Vector2f(0.99f);
		const Vector2f inputC = Vector2f(9.9f);
		const Vector2i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector2i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector2i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA != Vector2i(1));
		CHECK(outputB != Vector2i(1));
		CHECK(outputC != Vector2i(1));
	}

	{
		INFO("Outside: Top Right.");

		const Vector2f inputA = Vector2f(.2f);
		const Vector2f inputB = Vector2f(2.0f);
		const Vector2f inputC = Vector2f(20.0f);
		const Vector2i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector2i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector2i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA != Vector2i(1));
		CHECK(outputB != Vector2i(1));
		CHECK(outputC != Vector2i(1));
	}
}

TEST_CASE("math::Vector3f::ToGridPos")
{
	{
		INFO("Origin.");

		const Vector3f input = Vector3f::Zero;
		const Vector3i outputA = math::ToGridPos(input, 0.1f);
		const Vector3i outputB = math::ToGridPos(input, 1.0f);
		const Vector3i outputC = math::ToGridPos(input, 10.f);
		CHECK(outputA == Vector3i::Zero);
		CHECK(outputB == Vector3i::Zero);
		CHECK(outputC == Vector3i::Zero);
	}

	{
		INFO("Inside: Centre.");

		const Vector3f inputA = Vector3f(00.15f);
		const Vector3f inputB = Vector3f(01.5f);
		const Vector3f inputC = Vector3f(15.f);
		const Vector3i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector3i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector3i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA == Vector3i(1));
		CHECK(outputB == Vector3i(1));
		CHECK(outputC == Vector3i(1));
	}

	{
		INFO("Inside: Bottom Left.");

		const Vector3f inputA = Vector3f(.1f);
		const Vector3f inputB = Vector3f(1.0f);
		const Vector3f inputC = Vector3f(10.0f);
		const Vector3i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector3i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector3i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA == Vector3i(1));
		CHECK(outputB == Vector3i(1));
		CHECK(outputC == Vector3i(1));
	}

	{
		INFO("Inside: Top Right.");

		const Vector3f inputA = Vector3f(00.199f);
		const Vector3f inputB = Vector3f(01.99f);
		const Vector3f inputC = Vector3f(19.9f);
		const Vector3i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector3i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector3i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA == Vector3i(1));
		CHECK(outputB == Vector3i(1));
		CHECK(outputC == Vector3i(1));
	}

	{
		INFO("Outside: Bottom Left.");

		const Vector3f inputA = Vector3f(.099f);
		const Vector3f inputB = Vector3f(0.99f);
		const Vector3f inputC = Vector3f(9.9f);
		const Vector3i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector3i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector3i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA != Vector3i(1));
		CHECK(outputB != Vector3i(1));
		CHECK(outputC != Vector3i(1));
	}

	{
		INFO("Outside: Top Right.");

		const Vector3f inputA = Vector3f(.2f);
		const Vector3f inputB = Vector3f(2.0f);
		const Vector3f inputC = Vector3f(20.0f);
		const Vector3i outputA = math::ToGridPos(inputA, 0.1f);
		const Vector3i outputB = math::ToGridPos(inputB, 1.0f);
		const Vector3i outputC = math::ToGridPos(inputC, 10.f);
		CHECK(outputA != Vector3i(1));
		CHECK(outputB != Vector3i(1));
		CHECK(outputC != Vector3i(1));
	}
}

TEST_CASE("math::Vector2i::ToWorldPos")
{
}

TEST_CASE("math::Vector3i::ToWorldPos")
{
}