#include <Catch2/catch.hpp>

#include <Core/VectorHelpers.h>

TEST_CASE("VectorHelpers")
{
	SECTION("Angle(Vector2f, Vector2f)")
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

	SECTION("Angle(Vector3f, Vector3f)")
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

	SECTION("IsNearly(Vector2f, Vector2f)")
	{
	}

	SECTION("IsNearly(Vector3f, Vector3f)")
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
			CHECK(IsNearly(a, b, 0.001f));
		}

		{
			INFO("Nearly: Far from Origin.");

			const float offsetA = 0.1f;
			const float offsetB = 0.4f;
			const Vector3f a = Vector3f(KINDA_LARGE_FLOAT + offsetA, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
			const Vector3f b = Vector3f(KINDA_LARGE_FLOAT + offsetB, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
			CHECK(IsNearly(a, b, 0.01f));
		}

		{
			INFO("Not Nearly: Near Origin.");

			const float offsetA = 0.1f;
			const float offsetB = 0.14f;
			const Vector3f a = Vector3f(1.23f + offsetA, 4.56f, 7.89f);
			const Vector3f b = Vector3f(1.23f + offsetB, 4.56f, 7.89f);
			CHECK(!IsNearly(a, b, 0.001f));
		}

		{
			INFO("Not Nearly: Far from Origin.");

			const float offsetA = 0.1f;
			const float offsetB = 0.5f;
			const Vector3f a = Vector3f(KINDA_LARGE_FLOAT + offsetA, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
			const Vector3f b = Vector3f(KINDA_LARGE_FLOAT + offsetB, KINDA_LARGE_FLOAT, KINDA_LARGE_FLOAT);
			CHECK(!IsNearly(a, b, 0.01f));
		}
	}

	SECTION("ToGridPos(Vector2f)")
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

	SECTION("ToGridPos(Vector3f)")
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

	SECTION("ToWorldPos(Vector2i)")
	{
	}

	SECTION("ToWorldPos(Vector3i)")
	{
	}

	SECTION("ProjectOntoLine(Vector3f)")
	{
	}

	SECTION("ProjectOntoLineClamped(Vector3f)")
	{
	}

	SECTION("ProjectOntoLineXZ(Vector3f)")
	{
	}

	SECTION("ProjectOntoLineXZClamped(Vector3f)")
	{
	}

	SECTION("ProjectOntoRay(Vector3f)")
	{
	}

	SECTION("ProjectOntoRayXZ(Vector3f)")
	{
	}
}