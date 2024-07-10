#include <Catch2/catch.hpp>

#include "Core/Algorithms.h"
#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Types.h"

// #todo: const vs non-const

TEST_CASE("core::Contains")
{
	SECTION("Array")
	{
		const Array<int32> values = { 1, 2, 3 };
		CHECK(!core::Contains(values, 0));
		CHECK(core::Contains(values, 1));
		CHECK(core::Contains(values, 2));
		CHECK(core::Contains(values, 3));
		CHECK(!core::Contains(values, 4));
	}

	SECTION("Set")
	{
		const Set<int32> values = { 1, 2, 3 };
		CHECK(!core::Contains(values, 0));
		CHECK(core::Contains(values, 1));
		CHECK(core::Contains(values, 2));
		CHECK(core::Contains(values, 3));
		CHECK(!core::Contains(values, 4));
	}

	SECTION("Map")
	{
		const Map<int32, int32> values = { { 1, 1337 }, { 2, 666 } };
		CHECK(!core::Contains(values, 0));
		CHECK(core::Contains(values, 1));
	}
}

TEST_CASE("core::ContainsIf")
{
	struct Struct
	{
		bool operator< (const Struct& rhs) const { return m_Value < rhs.m_Value; }
		int32 m_Value;
	};

	SECTION("Array")
	{
		const Array<Struct> values = { { 1 }, { 2 }, { 3 } };
		CHECK(!core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
		CHECK(core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
		CHECK(core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
		CHECK(core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
		CHECK(!core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
	}

	SECTION("Set")
	{
		const Set<Struct> values = { { 1 }, { 2 }, { 3 } };
		CHECK(!core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
		CHECK(core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
		CHECK(core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
		CHECK(core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
		CHECK(!core::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
	}

	SECTION("Map")
	{
	}
}

TEST_CASE("core::Difference")
{
	SECTION("Array")
	{
		Array<int32> inputA = { 1, 2, 3 };
		Array<int32> inputB = { 2, 3, 4 };

		INFO("Array only contains values that were present in inputA, but not in inputB.");
		Array<int32> outputA;
		core::Difference(inputA, inputB, outputA);
		CHECK(core::Contains(outputA, 1));
		CHECK(!core::Contains(outputA, 2));
		CHECK(!core::Contains(outputA, 3));
		CHECK(!core::Contains(outputA, 4));

		INFO("Array only contains values that were present in inputB, but not in inputA.");
		Array<int32> outputB;
		core::Difference(inputB, inputA, outputB);
		CHECK(!core::Contains(outputB, 1));
		CHECK(!core::Contains(outputB, 2));
		CHECK(!core::Contains(outputB, 3));
		CHECK(core::Contains(outputB, 4));
	}

	SECTION("Set")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 2, 3, 4 };

		INFO("Set only contains values that were present in inputA, but not in inputB.");
		Set<int32> outputA;
		core::Difference(inputA, inputB, outputA);
		CHECK(core::Contains(outputA, 1));
		CHECK(!core::Contains(outputA, 2));
		CHECK(!core::Contains(outputA, 3));
		CHECK(!core::Contains(outputA, 4));

		INFO("Set only contains values that were present in inputB, but not in inputA.");
		Set<int32> outputB;
		core::Difference(inputB, inputA, outputB);
		CHECK(!core::Contains(outputB, 1));
		CHECK(!core::Contains(outputB, 2));
		CHECK(!core::Contains(outputB, 3));
		CHECK(core::Contains(outputB, 4));
	}

	SECTION("Map")
	{
		//// #todo: Map version of Contains
		//Map<int32, int32> inputA = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
		//Map<int32, int32> inputB = { { 2, 2 }, { 3, 3 }, { 4, 4 } };

		//INFO("Map only contains values that were present in inputA, but not in inputB.");
		//Map<int32, int32> outputA;
		//core::Difference(inputA, inputB, outputA);
		//CHECK(core::Contains(outputA, 1));
		//CHECK(!core::Contains(outputA, 2));
		//CHECK(!core::Contains(outputA, 3));
		//CHECK(!core::Contains(outputA, 4));

		//INFO("Map only contains values that were present in inputB, but not in inputA.");
		//Map<int32, int32> outputB;
		//core::Difference(inputB, inputA, outputB);
		//CHECK(!core::Contains(outputB, 1));
		//CHECK(!core::Contains(outputB, 2));
		//CHECK(!core::Contains(outputB, 3));
		//CHECK(core::Contains(outputB, 4));
	}
}

TEST_CASE("core::Intersection")
{
	SECTION("Array")
	{
		Array<int32> inputA = { 1, 2, 3 };
		Array<int32> inputB = { 2, 3, 4 };

		Array<int32> output;
		core::Intersection(inputA, inputB, output);
		CHECK(core::Contains(output, 2));
		CHECK(core::Contains(output, 3));
		CHECK(!core::Contains(output, 1));
		CHECK(!core::Contains(output, 4));
	}

	SECTION("Set")
	{
		Set<int32> inputA = { 1, 2, 3 };
		Set<int32> inputB = { 2, 3, 4 };

		Set<int32> output;
		core::Intersection(inputA, inputB, output);
		CHECK(core::Contains(output, 2));
		CHECK(core::Contains(output, 3));
		CHECK(!core::Contains(output, 1));
		CHECK(!core::Contains(output, 4));
	}

	SECTION("Map")
	{
		//Map<int32, int32> inputA = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
		//Map<int32, int32> inputB = { { 2, 2 }, { 3, 3 }, { 4, 4 } };

		//Map<int32, int32> output;
		//core::Intersection(inputA, inputB, output);
		//CHECK(!core::Contains(output, 1));
		//CHECK(core::Contains(output, 2));
		//CHECK(core::Contains(output, 3));
		//CHECK(!core::Contains(output, 4));
	}
}

TEST_CASE("enumerate::BeginForward")
{
	SECTION("Array")
	{
	}

	SECTION("Set")
	{
	}

	SECTION("Map")
	{
	}
}

TEST_CASE("enumerate::BeginReverse")
{
	SECTION("Array")
	{
	}

	SECTION("Set")
	{
	}

	SECTION("Map")
	{
	}
}

TEST_CASE("enumerate::EndForward")
{
	SECTION("Array")
	{
	}

	SECTION("Set")
	{
	}

	SECTION("Map")
	{
	}
}

TEST_CASE("enumerate::EndReverse")
{
	SECTION("Array")
	{
	}

	SECTION("Set")
	{
	}

	SECTION("Map")
	{
	}
}

TEST_CASE("enumerate::Forward")
{
	SECTION("Array")
	{
		int32 i = 0;
		Array<int32> values = { 0, 10, 20 };
		for (auto&& [index, value] : enumerate::Forward(values))
		{
			CHECK(index == i);
			CHECK(value == i * 10);
			i++;
		}

		INFO("Empty Array doesn't enumerate.");
		values.RemoveAll();
		for (auto&& [index, value] : enumerate::Forward(values))
			FAIL();
	}

	SECTION("Set")
	{
		int32 i = 0;
		Set<int32> values = { 0, 10, 20 };
		for (auto&& [index, value] : enumerate::Forward(values))
		{
			CHECK(index == i);
			CHECK(value == i * 10);
			i++;
		}

		INFO("Empty Set doesn't enumerate.");
		values.RemoveAll();
		for (auto&& [index, value] : enumerate::Forward(values))
			FAIL();
	}

	SECTION("Map")
	{
	}
}

TEST_CASE("enumerate::Reverse")
{
	SECTION("Array")
	{
		int32 i = 2;
		Array<int32> values = { 0, 10, 20 };
		for (auto&& [index, value] : enumerate::Reverse(values))
		{
			CHECK(index == i);
			CHECK(value == i * 10);
			i--;
		}

		INFO("Empty Array doesn't enumerate.");
		values = { };
		for (auto&& [index, value] : enumerate::Reverse(values))
			FAIL();
	}

	SECTION("Set")
	{
		int32 i = 2;
		Set<int32> values = { 0, 10, 20 };
		for (auto&& [index, value] : enumerate::Reverse(values))
		{
			CHECK(index == i);
			CHECK(value == i * 10);
			i--;
		}

		INFO("Empty Set doesn't enumerate.");
		values.RemoveAll();
		for (auto&& [index, value] : enumerate::Reverse(values))
			FAIL();
	}

	SECTION("Map")
	{
	}
}