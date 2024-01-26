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

TEST_CASE("math::Vector3f::Clamp")
{
	const Vector3f min = Vector3f(1.f, 2.f, 3.f);
	const Vector3f max = Vector3f(7.f, 8.f, 9.f);

	// nothing clamped
	CHECK(math::Clamp(Vector3f(4.f, 5.f, 6.f), min, max) == Vector3f(4.f, 5.f, 6.f));

	// x, y and z clamped to max
	CHECK(math::Clamp(Vector3f(+10.f, +20.f, +30.f), min, max) == max);
	// x only clamped to max
	CHECK(math::Clamp(Vector3f(+10.f, 5.f, 6.f), min, max) == Vector3f(max.x, 5.f, 6.f));
	// y only clamped to max
	CHECK(math::Clamp(Vector3f(4.f, +20.f, 6.f), min, max) == Vector3f(4.f, max.y, 6.f));
	// z only clamped to max
	CHECK(math::Clamp(Vector3f(4.f, 5.f, +30.f), min, max) == Vector3f(4.f, 5.f, max.z));

	// x, y and z clamped to min
	CHECK(math::Clamp(Vector3f(-10.f, -20.f, -30.f), min, max) == min);
	// x only clamped to min
	CHECK(math::Clamp(Vector3f(-10.f, 5.f, 6.f), min, max) == Vector3f(min.x, 5.f, 6.f));
	// y only clamped to min
	CHECK(math::Clamp(Vector3f(4.f, -20.f, 6.f), min, max) == Vector3f(4.f, min.y, 6.f));
	// z only clamped to min
	CHECK(math::Clamp(Vector3f(4.f, 5.f, -30.f), min, max) == Vector3f(4.f, 5.f, min.z));
}

TEST_CASE("math::Vector2f::ClampLength")
{
	const Vector2f vectorA = math::ClampLength(Vector2f::Zero, -1.f);
	CHECK(std::isnan(vectorA.Length()));

	const Vector2f vectorB = math::ClampLength(Vector2f::Zero, 0.f);
	CHECK(vectorB.Length() == 0.f);
	CHECK(vectorB == Vector2f::AxisX * 0.f);

	const Vector2f vectorC = math::ClampLength(Vector2f::AxisX, 0.1f);
	CHECK(vectorC.Length() == 0.1f);
	CHECK(vectorC == Vector2f::AxisX * 0.1f);

	const Vector2f vectorD = math::ClampLength(Vector2f::AxisY, 1.f);
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector2f::AxisY * 1.f);

	const Vector2f vectorE = math::ClampLength(Vector2f::AxisX, 10.f);
	CHECK(vectorE.Length() == 1.f);
	CHECK(vectorE == Vector2f::AxisX * 1.f);

	const Vector2f vectorF = math::ClampLength(Vector2f::AxisY * 10000000.f, 500.f);
	CHECK(vectorF.Length() == 500.f);
	CHECK(vectorF == Vector2f::AxisY * 500.f);
}

TEST_CASE("math::Vector3f::ClampLength")
{
	const Vector3f vectorA = math::ClampLength(Vector3f::Zero, -1.f);
	CHECK(std::isnan(vectorA.Length()));

	const Vector3f vectorB = math::ClampLength(Vector3f::Zero, 0.f);
	CHECK(vectorB.Length() == 0.f);
	CHECK(vectorB == Vector3f::AxisX * 0.f);

	const Vector3f vectorC = math::ClampLength(Vector3f::AxisX, 0.1f);
	CHECK(vectorC.Length() == 0.1f);
	CHECK(vectorC == Vector3f::AxisX * 0.1f);

	const Vector3f vectorD = math::ClampLength(Vector3f::AxisY, 1.f);
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector3f::AxisY * 1.f);

	const Vector3f vectorE = math::ClampLength(Vector3f::AxisX, 10.f);
	CHECK(vectorE.Length() == 1.f);
	CHECK(vectorE == Vector3f::AxisX * 1.f);

	const Vector3f vectorF = math::ClampLength(Vector3f::AxisY * 10000000.f, 500.f);
	CHECK(vectorF.Length() == 500.f);
	CHECK(vectorF == Vector3f::AxisY * 500.f);
}

TEST_CASE("math::Vector2f::Cross")
{
	CHECK(math::Cross(Vector2f::AxisX, +Vector2f::AxisX) == +0.f);
	CHECK(math::Cross(Vector2f::AxisX, +Vector2f::AxisY) == +1.f);
	CHECK(math::Cross(Vector2f::AxisX, -Vector2f::AxisX) == +0.f);
	CHECK(math::Cross(Vector2f::AxisX, -Vector2f::AxisY) == -1.f);
}

TEST_CASE("math::Vector3f::Cross")
{
	CHECK(math::Cross(Vector3f::AxisX, Vector3f::AxisY) == Vector3f::AxisZ);
	CHECK(math::Cross(Vector3f::AxisY, Vector3f::AxisZ) == Vector3f::AxisX);
	CHECK(math::Cross(Vector3f::AxisZ, Vector3f::AxisX) == Vector3f::AxisY);
}

TEST_CASE("math::Vector2f::Distance")
{
	CHECK(math::Distance(Vector2f::Zero, Vector2f::Zero) == 0.f);
	CHECK(math::Distance(Vector2f::Zero, Vector2f::AxisX) == 1.f);
	CHECK(math::Distance(Vector2f::Zero, Vector2f::AxisY) == 1.f);
	CHECK(math::Distance(Vector2f::Zero, Vector2f(10.f, 0.f)) == 10.f);
	CHECK(math::Distance(Vector2f::Zero, Vector2f(0.f, 10.f)) == 10.f);
}

TEST_CASE("math::Vector3f::Distance")
{
	CHECK(math::Distance(Vector3f::Zero, Vector3f::Zero) == 0.f);
	CHECK(math::Distance(Vector3f::Zero, Vector3f::AxisX) == 1.f);
	CHECK(math::Distance(Vector3f::Zero, Vector3f::AxisY) == 1.f);
	CHECK(math::Distance(Vector3f::Zero, Vector3f::AxisZ) == 1.f);
	CHECK(math::Distance(Vector3f::Zero, Vector3f(10.f, 0.f, 0.f)) == 10.f);
	CHECK(math::Distance(Vector3f::Zero, Vector3f(0.f, 10.f, 0.f)) == 10.f);
	CHECK(math::Distance(Vector3f::Zero, Vector3f(0.f, 0.f, 10.f)) == 10.f);
}

TEST_CASE("math::Vector2f::DistanceSqr")
{
	CHECK(math::DistanceSqr(Vector2f::Zero, Vector2f::Zero) == 0.f);
	CHECK(math::DistanceSqr(Vector2f::Zero, Vector2f::AxisX) == 1.f);
	CHECK(math::DistanceSqr(Vector2f::Zero, Vector2f::AxisY) == 1.f);
	CHECK(math::DistanceSqr(Vector2f::Zero, Vector2f(10.f, 0.f)) == 100.f);
	CHECK(math::DistanceSqr(Vector2f::Zero, Vector2f(0.f, 10.f)) == 100.f);
}

TEST_CASE("math::Vector3f::DistanceSqr")
{
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f::Zero) == 0.f);
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f::AxisX) == 1.f);
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f::AxisY) == 1.f);
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f::AxisZ) == 1.f);
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f(10.f, 0.f, 0.f)) == 100.f);
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f(0.f, 10.f, 0.f)) == 100.f);
	CHECK(math::DistanceSqr(Vector3f::Zero, Vector3f(0.f, 0.f, 10.f)) == 100.f);
}

TEST_CASE("math::Vector2f::Dot")
{
	CHECK(math::Dot(Vector2f::AxisX, +Vector2f::AxisX) == +1.f);
	CHECK(math::Dot(Vector2f::AxisX, +Vector2f::AxisY) == +0.f);
	CHECK(math::Dot(Vector2f::AxisX, -Vector2f::AxisX) == -1.f);
	CHECK(math::Dot(Vector2f::AxisX, -Vector2f::AxisY) == +0.f);
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
	CHECK(math::Divide(Vector2f(+1.f, +2.f), Vector2f(+4.f, +8.f)) == Vector2f(+0.25f, +0.25f));
	CHECK(math::Divide(Vector2f(+4.f, +8.f), Vector2f(+1.f, +2.f)) == Vector2f(+4.00f, +4.00f));
	CHECK(math::Divide(Vector2f(+1.f, +8.f), Vector2f(+4.f, +2.f)) == Vector2f(+0.25f, +4.00f));
	CHECK(math::Divide(Vector2f(+4.f, +2.f), Vector2f(+1.f, +8.f)) == Vector2f(+4.00f, +0.25f));

	CHECK(math::Divide(Vector2f(-1.f, -2.f), Vector2f(-4.f, -8.f)) == Vector2f(+0.25f, +0.25f));
	CHECK(math::Divide(Vector2f(-4.f, -8.f), Vector2f(-1.f, -2.f)) == Vector2f(+4.00f, +4.00f));
	CHECK(math::Divide(Vector2f(-1.f, -8.f), Vector2f(-4.f, -2.f)) == Vector2f(+0.25f, +4.00f));
	CHECK(math::Divide(Vector2f(-4.f, -2.f), Vector2f(-1.f, -8.f)) == Vector2f(+4.00f, +0.25f));
}

TEST_CASE("math::Vector3f::Divide")
{
	CHECK(math::Divide(Vector3f(+1.f, +2.f, +3.f), Vector3f(+4.f, +5.f, +6.f)) == Vector3f(+0.25f, +0.40f, +0.50f));
	CHECK(math::Divide(Vector3f(+4.f, +5.f, +6.f), Vector3f(+1.f, +2.f, +3.f)) == Vector3f(+4.00f, +2.50f, +2.00f));
	CHECK(math::Divide(Vector3f(+1.f, +2.f, +6.f), Vector3f(+4.f, +5.f, +3.f)) == Vector3f(+0.25f, +0.40f, +2.00f));
	CHECK(math::Divide(Vector3f(+1.f, +5.f, +3.f), Vector3f(+4.f, +2.f, +6.f)) == Vector3f(+0.25f, +2.50f, +0.50f));
	CHECK(math::Divide(Vector3f(+4.f, +2.f, +3.f), Vector3f(+1.f, +5.f, +6.f)) == Vector3f(+4.00f, +0.40f, +0.50f));

	CHECK(math::Divide(Vector3f(-1.f, -2.f, -3.f), Vector3f(-4.f, -5.f, -6.f)) == Vector3f(+0.25f, +0.40f, +0.50f));
	CHECK(math::Divide(Vector3f(-4.f, -5.f, -6.f), Vector3f(-1.f, -2.f, -3.f)) == Vector3f(+4.00f, +2.50f, +2.00f));
	CHECK(math::Divide(Vector3f(-1.f, -2.f, -6.f), Vector3f(-4.f, -5.f, -3.f)) == Vector3f(+0.25f, +0.40f, +2.00f));
	CHECK(math::Divide(Vector3f(-1.f, -5.f, -3.f), Vector3f(-4.f, -2.f, -6.f)) == Vector3f(+0.25f, +2.50f, +0.50f));
	CHECK(math::Divide(Vector3f(-4.f, -2.f, -3.f), Vector3f(-1.f, -5.f, -6.f)) == Vector3f(+4.00f, +0.40f, +0.50f));
}

TEST_CASE("math::Vector2f::Max")
{
	CHECK(math::Max(Vector2f(+1.f, +2.f), Vector2f(+3.f, +4.f)) == Vector2f(+3.f, +4.f));
	CHECK(math::Max(Vector2f(+3.f, +4.f), Vector2f(+1.f, +2.f)) == Vector2f(+3.f, +4.f));
	CHECK(math::Max(Vector2f(+1.f, +4.f), Vector2f(+3.f, +2.f)) == Vector2f(+3.f, +4.f));
	CHECK(math::Max(Vector2f(+3.f, +2.f), Vector2f(+1.f, +4.f)) == Vector2f(+3.f, +4.f));

	CHECK(math::Max(Vector2f(-1.f, -2.f), Vector2f(-3.f, -4.f)) == Vector2f(-1.f, -2.f));
	CHECK(math::Max(Vector2f(-3.f, -4.f), Vector2f(-1.f, -2.f)) == Vector2f(-1.f, -2.f));
	CHECK(math::Max(Vector2f(-1.f, -4.f), Vector2f(-3.f, -2.f)) == Vector2f(-1.f, -2.f));
	CHECK(math::Max(Vector2f(-3.f, -2.f), Vector2f(-1.f, -4.f)) == Vector2f(-1.f, -2.f));
}

TEST_CASE("math::Vector3f::Max")
{
	CHECK(math::Max(Vector3f(+1.f, +2.f, +3.f), Vector3f(+4.f, +5.f, +6.f)) == Vector3f(+4.f, +5.f, +6.f));
	CHECK(math::Max(Vector3f(+4.f, +5.f, +6.f), Vector3f(+1.f, +2.f, +3.f)) == Vector3f(+4.f, +5.f, +6.f));
	CHECK(math::Max(Vector3f(+1.f, +2.f, +6.f), Vector3f(+4.f, +5.f, +3.f)) == Vector3f(+4.f, +5.f, +6.f));
	CHECK(math::Max(Vector3f(+1.f, +5.f, +3.f), Vector3f(+4.f, +2.f, +6.f)) == Vector3f(+4.f, +5.f, +6.f));
	CHECK(math::Max(Vector3f(+4.f, +2.f, +3.f), Vector3f(+1.f, +5.f, +6.f)) == Vector3f(+4.f, +5.f, +6.f));

	CHECK(math::Max(Vector3f(-1.f, -2.f, -3.f), Vector3f(-4.f, -5.f, -6.f)) == Vector3f(-1.f, -2.f, -3.f));
	CHECK(math::Max(Vector3f(-4.f, -5.f, -6.f), Vector3f(-1.f, -2.f, -3.f)) == Vector3f(-1.f, -2.f, -3.f));
	CHECK(math::Max(Vector3f(-1.f, -2.f, -6.f), Vector3f(-4.f, -5.f, -3.f)) == Vector3f(-1.f, -2.f, -3.f));
	CHECK(math::Max(Vector3f(-1.f, -5.f, -3.f), Vector3f(-4.f, -2.f, -6.f)) == Vector3f(-1.f, -2.f, -3.f));
	CHECK(math::Max(Vector3f(-4.f, -2.f, -3.f), Vector3f(-1.f, -5.f, -6.f)) == Vector3f(-1.f, -2.f, -3.f));
}

TEST_CASE("math::Vector2f::Min")
{
	CHECK(math::Min(Vector2f(+1.f, +2.f), Vector2f(+3.f, +4.f)) == Vector2f(+1.f, +2.f));
	CHECK(math::Min(Vector2f(+3.f, +4.f), Vector2f(+1.f, +2.f)) == Vector2f(+1.f, +2.f));
	CHECK(math::Min(Vector2f(+1.f, +4.f), Vector2f(+3.f, +2.f)) == Vector2f(+1.f, +2.f));
	CHECK(math::Min(Vector2f(+3.f, +2.f), Vector2f(+1.f, +4.f)) == Vector2f(+1.f, +2.f));

	CHECK(math::Min(Vector2f(-1.f, -2.f), Vector2f(-3.f, -4.f)) == Vector2f(-3.f, -4.f));
	CHECK(math::Min(Vector2f(-3.f, -4.f), Vector2f(-1.f, -2.f)) == Vector2f(-3.f, -4.f));
	CHECK(math::Min(Vector2f(-1.f, -4.f), Vector2f(-3.f, -2.f)) == Vector2f(-3.f, -4.f));
	CHECK(math::Min(Vector2f(-3.f, -2.f), Vector2f(-1.f, -4.f)) == Vector2f(-3.f, -4.f));
}

TEST_CASE("math::Vector3f::Min")
{
	CHECK(math::Min(Vector3f(+1.f, +2.f, +3.f), Vector3f(+4.f, +5.f, +6.f)) == Vector3f(+1.f, +2.f, +3.f));
	CHECK(math::Min(Vector3f(+4.f, +5.f, +6.f), Vector3f(+1.f, +2.f, +3.f)) == Vector3f(+1.f, +2.f, +3.f));
	CHECK(math::Min(Vector3f(+1.f, +2.f, +6.f), Vector3f(+4.f, +5.f, +3.f)) == Vector3f(+1.f, +2.f, +3.f));
	CHECK(math::Min(Vector3f(+1.f, +5.f, +3.f), Vector3f(+4.f, +2.f, +6.f)) == Vector3f(+1.f, +2.f, +3.f));
	CHECK(math::Min(Vector3f(+4.f, +2.f, +3.f), Vector3f(+1.f, +5.f, +6.f)) == Vector3f(+1.f, +2.f, +3.f));

	CHECK(math::Min(Vector3f(-1.f, -2.f, -3.f), Vector3f(-4.f, -5.f, -6.f)) == Vector3f(-4.f, -5.f, -6.f));
	CHECK(math::Min(Vector3f(-4.f, -5.f, -6.f), Vector3f(-1.f, -2.f, -3.f)) == Vector3f(-4.f, -5.f, -6.f));
	CHECK(math::Min(Vector3f(-1.f, -2.f, -6.f), Vector3f(-4.f, -5.f, -3.f)) == Vector3f(-4.f, -5.f, -6.f));
	CHECK(math::Min(Vector3f(-1.f, -5.f, -3.f), Vector3f(-4.f, -2.f, -6.f)) == Vector3f(-4.f, -5.f, -6.f));
	CHECK(math::Min(Vector3f(-4.f, -2.f, -3.f), Vector3f(-1.f, -5.f, -6.f)) == Vector3f(-4.f, -5.f, -6.f));
}

TEST_CASE("math::Vector2f::Multiply")
{
	CHECK(math::Multiply(Vector2f(+1.f, +2.f), Vector2f(+3.f, +4.f)) == Vector2f(+3.f, +8.f));
	CHECK(math::Multiply(Vector2f(+3.f, +4.f), Vector2f(+1.f, +2.f)) == Vector2f(+3.f, +8.f));
	CHECK(math::Multiply(Vector2f(+1.f, +4.f), Vector2f(+3.f, +2.f)) == Vector2f(+3.f, +8.f));
	CHECK(math::Multiply(Vector2f(+3.f, +2.f), Vector2f(+1.f, +4.f)) == Vector2f(+3.f, +8.f));

	CHECK(math::Multiply(Vector2f(-1.f, -2.f), Vector2f(-3.f, -4.f)) == Vector2f(+3.f, +8.f));
	CHECK(math::Multiply(Vector2f(-3.f, -4.f), Vector2f(-1.f, -2.f)) == Vector2f(+3.f, +8.f));
	CHECK(math::Multiply(Vector2f(-1.f, -4.f), Vector2f(-3.f, -2.f)) == Vector2f(+3.f, +8.f));
	CHECK(math::Multiply(Vector2f(-3.f, -2.f), Vector2f(-1.f, -4.f)) == Vector2f(+3.f, +8.f));
}

TEST_CASE("math::Vector3f::Multiply")
{
	CHECK(math::Multiply(Vector3f(+1.f, +2.f, +3.f), Vector3f(+4.f, +5.f, +6.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(+4.f, +5.f, +6.f), Vector3f(+1.f, +2.f, +3.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(+1.f, +2.f, +6.f), Vector3f(+4.f, +5.f, +3.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(+1.f, +5.f, +3.f), Vector3f(+4.f, +2.f, +6.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(+4.f, +2.f, +3.f), Vector3f(+1.f, +5.f, +6.f)) == Vector3f(+4.f, +10.f, +18.f));

	CHECK(math::Multiply(Vector3f(-1.f, -2.f, -3.f), Vector3f(-4.f, -5.f, -6.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(-4.f, -5.f, -6.f), Vector3f(-1.f, -2.f, -3.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(-1.f, -2.f, -6.f), Vector3f(-4.f, -5.f, -3.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(-1.f, -5.f, -3.f), Vector3f(-4.f, -2.f, -6.f)) == Vector3f(+4.f, +10.f, +18.f));
	CHECK(math::Multiply(Vector3f(-4.f, -2.f, -3.f), Vector3f(-1.f, -5.f, -6.f)) == Vector3f(+4.f, +10.f, +18.f));
}

TEST_CASE("math::Vector2f::Perpendicular")
{
	const Vector2f vectorA = math::Perpendicular(Vector2f::AxisX);
	const Vector2f vectorB = math::Perpendicular(vectorA);
	const Vector2f vectorC = math::Perpendicular(vectorB);
	const Vector2f vectorD = math::Perpendicular(vectorC);

	CHECK(vectorA == -Vector2f::AxisY);
	CHECK(vectorB == -Vector2f::AxisX);
	CHECK(vectorC == +Vector2f::AxisY);
	CHECK(vectorD == +Vector2f::AxisX);
}

TEST_CASE("math::Vector3f::Project(Line)")
{
	// min
	const Vector3f vectorA = math::Project(Vector3f(0.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorB = math::Project(Vector3f(2.f, 0.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorC = math::Project(Vector3f(2.f, 2.f, 0.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorA == Vector3f::Zero);
	CHECK(vectorB == Vector3f::Zero);
	CHECK(vectorC == Vector3f::Zero);

	// mid
	const Vector3f vectorD = math::Project(Vector3f(.5f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorE = math::Project(Vector3f(2.f, .5f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorF = math::Project(Vector3f(2.f, 2.f, .5f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorD == Vector3f::AxisX * 0.5f);
	CHECK(vectorE == Vector3f::AxisY * 0.5f);
	CHECK(vectorF == Vector3f::AxisZ * 0.5f);

	// max
	const Vector3f vectorG = math::Project(Vector3f(1.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorH = math::Project(Vector3f(2.f, 1.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorI = math::Project(Vector3f(2.f, 2.f, 1.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorG == Vector3f::AxisX);
	CHECK(vectorH == Vector3f::AxisY);
	CHECK(vectorI == Vector3f::AxisZ);


	// min clamped
	const Vector3f vectorJ = math::Project(Vector3f(-1.f, +2.f, +2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorK = math::Project(Vector3f(+2.f, -1.f, +2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorL = math::Project(Vector3f(+2.f, +2.f, -1.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorJ == Vector3f::Zero);
	CHECK(vectorK == Vector3f::Zero);
	CHECK(vectorL == Vector3f::Zero);

	// max clamped
	const Vector3f vectorM = math::Project(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorN = math::Project(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorO = math::Project(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorM == Vector3f::AxisX);
	CHECK(vectorN == Vector3f::AxisY);
	CHECK(vectorO == Vector3f::AxisZ);
}

TEST_CASE("math::Vector3f::Project(Ray)")
{
	// min
	const Vector3f vectorA = math::Project(Vector3f(0.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorB = math::Project(Vector3f(2.f, 0.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorC = math::Project(Vector3f(2.f, 2.f, 0.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorA == Vector3f::Zero);
	CHECK(vectorB == Vector3f::Zero);
	CHECK(vectorC == Vector3f::Zero);

	// mid
	const Vector3f vectorD = math::Project(Vector3f(.5f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorE = math::Project(Vector3f(2.f, .5f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorF = math::Project(Vector3f(2.f, 2.f, .5f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorD == Vector3f::AxisX * 0.5f);
	CHECK(vectorE == Vector3f::AxisY * 0.5f);
	CHECK(vectorF == Vector3f::AxisZ * 0.5f);

	// max
	const Vector3f vectorG = math::Project(Vector3f(1.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorH = math::Project(Vector3f(2.f, 1.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorI = math::Project(Vector3f(2.f, 2.f, 1.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorG == Vector3f::AxisX);
	CHECK(vectorH == Vector3f::AxisY);
	CHECK(vectorI == Vector3f::AxisZ);


	// min clamped
	const Vector3f vectorJ = math::Project(Vector3f(-1.f, +2.f, +2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorK = math::Project(Vector3f(+2.f, -1.f, +2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorL = math::Project(Vector3f(+2.f, +2.f, -1.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorJ == Vector3f::AxisX * -1.f);
	CHECK(vectorK == Vector3f::AxisY * -1.f);
	CHECK(vectorL == Vector3f::AxisZ * -1.f);

	// max clamped
	const Vector3f vectorM = math::Project(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorN = math::Project(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorO = math::Project(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorM == Vector3f::AxisX * 2.f);
	CHECK(vectorN == Vector3f::AxisY * 2.f);
	CHECK(vectorO == Vector3f::AxisZ * 2.f);
}

TEST_CASE("math::Vector3f::ProjectXY(Line)")
{
	// min
	const Vector3f vectorA = math::ProjectXY(Vector3f(0.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorB = math::ProjectXY(Vector3f(2.f, 0.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorC = math::ProjectXY(Vector3f(2.f, 2.f, 0.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorA == Vector3f::Zero);
	CHECK(vectorB == Vector3f::Zero);
	CHECK(vectorC == Vector3f::Zero);

	// mid
	const Vector3f vectorD = math::ProjectXY(Vector3f(.5f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorE = math::ProjectXY(Vector3f(2.f, .5f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorF = math::ProjectXY(Vector3f(2.f, 2.f, .5f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorD == Vector3f::AxisX * 0.5f);
	CHECK(vectorE == Vector3f::AxisY * 0.5f);
	CHECK(vectorF == Vector3f::Zero);

	// max
	const Vector3f vectorG = math::ProjectXY(Vector3f(1.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorH = math::ProjectXY(Vector3f(2.f, 1.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorI = math::ProjectXY(Vector3f(2.f, 2.f, 1.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorG == Vector3f::AxisX);
	CHECK(vectorH == Vector3f::AxisY);
	CHECK(vectorI == Vector3f::Zero);


	// min un-clamped
	const Vector3f vectorJ = math::ProjectXY(Vector3f(-1.f, +2.f, +2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorK = math::ProjectXY(Vector3f(+2.f, -1.f, +2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorL = math::ProjectXY(Vector3f(+2.f, +2.f, -1.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorJ == Vector3f::Zero);
	CHECK(vectorK == Vector3f::Zero);
	CHECK(vectorL == Vector3f::Zero);

	// max un-clamped
	const Vector3f vectorM = math::ProjectXY(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorN = math::ProjectXY(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorO = math::ProjectXY(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorM == Vector3f::AxisX);
	CHECK(vectorN == Vector3f::AxisY);
	CHECK(vectorO == Vector3f::Zero);
}

TEST_CASE("math::Vector3f::ProjectXY(Ray)")
{
	// min
	const Vector3f vectorA = math::ProjectXY(Vector3f(0.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorB = math::ProjectXY(Vector3f(2.f, 0.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorC = math::ProjectXY(Vector3f(2.f, 2.f, 0.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorA == Vector3f::Zero);
	CHECK(vectorB == Vector3f::Zero);
	CHECK(vectorC == Vector3f::Zero);

	// mid
	const Vector3f vectorD = math::ProjectXY(Vector3f(.5f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorE = math::ProjectXY(Vector3f(2.f, .5f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorF = math::ProjectXY(Vector3f(2.f, 2.f, .5f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorD == Vector3f::AxisX * 0.5f);
	CHECK(vectorE == Vector3f::AxisY * 0.5f);
	CHECK(vectorF == Vector3f::Zero);

	// max
	const Vector3f vectorG = math::ProjectXY(Vector3f(1.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorH = math::ProjectXY(Vector3f(2.f, 1.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorI = math::ProjectXY(Vector3f(2.f, 2.f, 1.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorG == Vector3f::AxisX);
	CHECK(vectorH == Vector3f::AxisY);
	CHECK(vectorI == Vector3f::Zero);

	// min clamped
	const Vector3f vectorJ = math::ProjectXY(Vector3f(-1.f, +2.f, +2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorK = math::ProjectXY(Vector3f(+2.f, -1.f, +2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorL = math::ProjectXY(Vector3f(+2.f, +2.f, -1.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorJ == Vector3f::AxisX * -1.f);
	CHECK(vectorK == Vector3f::AxisY * -1.f);
	CHECK(vectorL == Vector3f::Zero);

	// max clamped
	const Vector3f vectorM = math::ProjectXY(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorN = math::ProjectXY(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorO = math::ProjectXY(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorM == Vector3f::AxisX * 2.f);
	CHECK(vectorN == Vector3f::AxisY * 2.f);
	CHECK(vectorO == Vector3f::Zero);
}

TEST_CASE("math::Vector3f::ProjectXZ(Line)")
{
	// min
	const Vector3f vectorA = math::ProjectXZ(Vector3f(0.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorB = math::ProjectXZ(Vector3f(2.f, 0.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorC = math::ProjectXZ(Vector3f(2.f, 2.f, 0.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorA == Vector3f::Zero);
	CHECK(vectorB == Vector3f::Zero);
	CHECK(vectorC == Vector3f::Zero);

	// mid
	const Vector3f vectorD = math::ProjectXZ(Vector3f(.5f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorE = math::ProjectXZ(Vector3f(2.f, .5f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorF = math::ProjectXZ(Vector3f(2.f, 2.f, .5f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorD == Vector3f::AxisX * 0.5f);
	CHECK(vectorE == Vector3f::Zero);
	CHECK(vectorF == Vector3f::AxisZ * 0.5f);

	// max
	const Vector3f vectorG = math::ProjectXZ(Vector3f(1.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorH = math::ProjectXZ(Vector3f(2.f, 1.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorI = math::ProjectXZ(Vector3f(2.f, 2.f, 1.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorG == Vector3f::AxisX);
	CHECK(vectorH == Vector3f::Zero);
	CHECK(vectorI == Vector3f::AxisZ);


	// min un-clamped
	const Vector3f vectorJ = math::ProjectXZ(Vector3f(-1.f, +2.f, +2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorK = math::ProjectXZ(Vector3f(+2.f, -1.f, +2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorL = math::ProjectXZ(Vector3f(+2.f, +2.f, -1.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorJ == Vector3f::Zero);
	CHECK(vectorK == Vector3f::Zero);
	CHECK(vectorL == Vector3f::Zero);

	// max un-clamped
	const Vector3f vectorM = math::ProjectXZ(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorN = math::ProjectXZ(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorO = math::ProjectXZ(Vector3f(2.f, 2.f, 2.f), Line(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorM == Vector3f::AxisX);
	CHECK(vectorN == Vector3f::Zero);
	CHECK(vectorO == Vector3f::AxisZ);
}

TEST_CASE("math::Vector3f::ProjectXZ(Ray)")
{
	// min
	const Vector3f vectorA = math::ProjectXZ(Vector3f(0.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorB = math::ProjectXZ(Vector3f(2.f, 0.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorC = math::ProjectXZ(Vector3f(2.f, 2.f, 0.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorA == Vector3f::Zero);
	CHECK(vectorB == Vector3f::Zero);
	CHECK(vectorC == Vector3f::Zero);

	// mid
	const Vector3f vectorD = math::ProjectXZ(Vector3f(.5f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorE = math::ProjectXZ(Vector3f(2.f, .5f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorF = math::ProjectXZ(Vector3f(2.f, 2.f, .5f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorD == Vector3f::AxisX * 0.5f);
	CHECK(vectorE == Vector3f::Zero);
	CHECK(vectorF == Vector3f::AxisZ * 0.5f);

	// max
	const Vector3f vectorG = math::ProjectXZ(Vector3f(1.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorH = math::ProjectXZ(Vector3f(2.f, 1.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorI = math::ProjectXZ(Vector3f(2.f, 2.f, 1.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorG == Vector3f::AxisX);
	CHECK(vectorH == Vector3f::Zero);
	CHECK(vectorI == Vector3f::AxisZ);


	// min un-clamped
	const Vector3f vectorJ = math::ProjectXZ(Vector3f(-1.f, +2.f, +2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorK = math::ProjectXZ(Vector3f(+2.f, -1.f, +2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorL = math::ProjectXZ(Vector3f(+2.f, +2.f, -1.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorJ == Vector3f::AxisX * -1.f);
	CHECK(vectorK == Vector3f::Zero);
	CHECK(vectorL == Vector3f::AxisZ * -1.f);

	// max un-clamped
	const Vector3f vectorM = math::ProjectXZ(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisX));
	const Vector3f vectorN = math::ProjectXZ(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisY));
	const Vector3f vectorO = math::ProjectXZ(Vector3f(2.f, 2.f, 2.f), Ray(Vector3f::Zero, Vector3f::AxisZ));
	CHECK(vectorM == Vector3f::AxisX * 2.f);
	CHECK(vectorN == Vector3f::Zero);
	CHECK(vectorO == Vector3f::AxisZ * 2.f);
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