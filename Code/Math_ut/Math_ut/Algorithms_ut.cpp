#include "MathPCH.h"

#include "Core/Array.h"
#include "Math/Algorithms.h"

TEST_CASE("enumerate::Length. Positive Length, Positive Stride.")
{
	Array<float> values;
	for (const float value : enumerate::Length(1.f, 4.f))
		values.Append(value);

	REQUIRE(values.GetCount() == 5);
	CHECK(values[0] == 0.f);
	CHECK(values[1] == 1.f);
	CHECK(values[2] == 2.f);
	CHECK(values[3] == 3.f);
	CHECK(values[4] == 4.f);
}

TEST_CASE("enumerate::Length. Positive Length, Negative Stride.")
{
	// will cause an infinite loop
	int32 count = 0;
	for (const float value : enumerate::Length(-1.f, 4.f))
	{
		count++;
		if (count == 1000)
			break;
	}

	CHECK(count == 1000);
}

TEST_CASE("enumerate::Length. Negative Length, Positive Stride.")
{
	for (const float value : enumerate::Length(1.f, -4.f))
		FAIL();
}

TEST_CASE("enumerate::Length. Negative Length, Negative Stride.")
{
	for (const float value : enumerate::Length(-1.f, -4.f))
		FAIL();
}

TEST_CASE("enumerate::Length. Un-even distribution.")
{
	Array<float> values;
	for (const float value : enumerate::Length(1.5f, 4.f))
		values.Append(value);

	REQUIRE(values.GetCount() == 4);
	CHECK(values[0] == 0.0f);
	CHECK(values[1] == 1.5f);
	CHECK(values[2] == 3.0f);
	CHECK(values[3] == 4.0f);
}
