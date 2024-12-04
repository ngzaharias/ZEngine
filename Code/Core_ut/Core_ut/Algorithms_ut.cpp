#include <Catch2/catch.hpp>

#include "Core/Algorithms.h"
#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Types.h"

// #todo: const vs non-const

namespace
{
	struct Struct
	{
		bool operator< (const Struct& rhs) const { return m_Value < rhs.m_Value; }
		int32 m_Value;
	};
}

TEST_CASE("enumerate::Contains. Array.")
{
	const Array<int32> values = { 1, 2, 3 };
	CHECK(!enumerate::Contains(values, 0));
	CHECK(enumerate::Contains(values, 1));
	CHECK(enumerate::Contains(values, 2));
	CHECK(enumerate::Contains(values, 3));
	CHECK(!enumerate::Contains(values, 4));
}

TEST_CASE("enumerate::Contains. Set.")
{
	const Set<int32> values = { 1, 2, 3 };
	CHECK(!enumerate::Contains(values, 0));
	CHECK(enumerate::Contains(values, 1));
	CHECK(enumerate::Contains(values, 2));
	CHECK(enumerate::Contains(values, 3));
	CHECK(!enumerate::Contains(values, 4));
}

TEST_CASE("enumerate::Contains. Map.")
{
	const Map<int32, int32> values = { { 1, 1337 }, { 2, 666 } };
	CHECK(!enumerate::Contains(values, 0));
	CHECK(enumerate::Contains(values, 1));
}

TEST_CASE("enumerate::ContainsIf. Array.")
{
	const Array<Struct> values = { { 1 }, { 2 }, { 3 } };
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
}

TEST_CASE("enumerate::ContainsIf. Set.")
{
	const Set<Struct> values = { { 1 }, { 2 }, { 3 } };
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
}

TEST_CASE("enumerate::ContainsIf. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::Difference. Array.")
{
	Array<int32> inputA = { 1, 2, 3 };
	Array<int32> inputB = { 2, 3, 4 };

	INFO("Array only contains values that were present in inputA, but not in inputB.");
	Array<int32> outputA;
	enumerate::Difference(inputA, inputB, outputA);
	CHECK(enumerate::Contains(outputA, 1));
	CHECK(!enumerate::Contains(outputA, 2));
	CHECK(!enumerate::Contains(outputA, 3));
	CHECK(!enumerate::Contains(outputA, 4));

	INFO("Array only contains values that were present in inputB, but not in inputA.");
	Array<int32> outputB;
	enumerate::Difference(inputB, inputA, outputB);
	CHECK(!enumerate::Contains(outputB, 1));
	CHECK(!enumerate::Contains(outputB, 2));
	CHECK(!enumerate::Contains(outputB, 3));
	CHECK(enumerate::Contains(outputB, 4));
}

TEST_CASE("enumerate::Difference. Set.")
{
	Set<int32> inputA = { 1, 2, 3 };
	Set<int32> inputB = { 2, 3, 4 };

	INFO("Set only contains values that were present in inputA, but not in inputB.");
	Set<int32> outputA;
	enumerate::Difference(inputA, inputB, outputA);
	CHECK(enumerate::Contains(outputA, 1));
	CHECK(!enumerate::Contains(outputA, 2));
	CHECK(!enumerate::Contains(outputA, 3));
	CHECK(!enumerate::Contains(outputA, 4));

	INFO("Set only contains values that were present in inputB, but not in inputA.");
	Set<int32> outputB;
	enumerate::Difference(inputB, inputA, outputB);
	CHECK(!enumerate::Contains(outputB, 1));
	CHECK(!enumerate::Contains(outputB, 2));
	CHECK(!enumerate::Contains(outputB, 3));
	CHECK(enumerate::Contains(outputB, 4));
}

TEST_CASE("enumerate::Difference. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::Intersection. Array.")
{
	Array<int32> inputA = { 1, 2, 3 };
	Array<int32> inputB = { 2, 3, 4 };

	Array<int32> output;
	enumerate::Intersection(inputA, inputB, output);
	CHECK(enumerate::Contains(output, 2));
	CHECK(enumerate::Contains(output, 3));
	CHECK(!enumerate::Contains(output, 1));
	CHECK(!enumerate::Contains(output, 4));
}

TEST_CASE("enumerate::Intersection. Set.")
{
	Set<int32> inputA = { 1, 2, 3 };
	Set<int32> inputB = { 2, 3, 4 };

	Set<int32> output;
	enumerate::Intersection(inputA, inputB, output);
	CHECK(enumerate::Contains(output, 2));
	CHECK(enumerate::Contains(output, 3));
	CHECK(!enumerate::Contains(output, 1));
	CHECK(!enumerate::Contains(output, 4));
}

TEST_CASE("enumerate::Intersection. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::BeginForward. Array.")
{
	CHECK(false);
}

TEST_CASE("enumerate::BeginForward. Set.")
{
	CHECK(false);
}

TEST_CASE("enumerate::BeginForward. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::BeginReverse. Array.")
{
	CHECK(false);
}

TEST_CASE("enumerate::BeginReverse. Set.")
{
	CHECK(false);
}

TEST_CASE("enumerate::BeginReverse. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::EndForward. Array.")
{
	CHECK(false);
}

TEST_CASE("enumerate::EndForward. Set.")
{
	CHECK(false);
}

TEST_CASE("enumerate::EndForward. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::EndReverse. Array.")
{
	CHECK(false);
}

TEST_CASE("enumerate::EndReverse. Set.")
{
	CHECK(false);
}

TEST_CASE("enumerate::EndReverse. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::Forward. Array.")
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

TEST_CASE("enumerate::Forward. Set.")
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

TEST_CASE("enumerate::Forward. Map.")
{
	CHECK(false);
}

TEST_CASE("enumerate::Reverse. Array.")
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

TEST_CASE("enumerate::Reverse. Set.")
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

TEST_CASE("enumerate::Reverse. Map.")
{
	CHECK(false);
}
