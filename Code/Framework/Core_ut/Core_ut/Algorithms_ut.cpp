#include "CorePCH.h"

#include "Core/Algorithms.h"
#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Types.h"

#define HELPER_TEST_CASE(name) TEST_CASE("enumerate::" name, "[enumerate]")

namespace
{
	struct Struct
	{
		bool operator< (const Struct& rhs) const { return m_Value < rhs.m_Value; }
		int32 m_Value;
	};
}

HELPER_TEST_CASE("Contains supports Array.")
{
	const Array<int32> values = { 1, 2, 3 };
	CHECK(!enumerate::Contains(values, 0));
	CHECK(enumerate::Contains(values, 1));
	CHECK(enumerate::Contains(values, 2));
	CHECK(enumerate::Contains(values, 3));
	CHECK(!enumerate::Contains(values, 4));
}

HELPER_TEST_CASE("Contains supports Set.")
{
	const Set<int32> values = { 1, 2, 3 };
	CHECK(!enumerate::Contains(values, 0));
	CHECK(enumerate::Contains(values, 1));
	CHECK(enumerate::Contains(values, 2));
	CHECK(enumerate::Contains(values, 3));
	CHECK(!enumerate::Contains(values, 4));
}

HELPER_TEST_CASE("Contains supports Map.")
{
	const Map<int32, int32> values = { { 1, 1337 }, { 2, 666 } };
	CHECK(!enumerate::Contains(values, 0));
	CHECK(enumerate::Contains(values, 1));
}

HELPER_TEST_CASE("ContainsIf supports Array.")
{
	const Array<Struct> values = { { 1 }, { 2 }, { 3 } };
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
}

HELPER_TEST_CASE("ContainsIf supports Set.")
{
	const Set<Struct> values = { { 1 }, { 2 }, { 3 } };
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
	CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
	CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
}

HELPER_TEST_CASE("ContainsIf supports Map.")
{
	//const Map<int32, Struct> values = { { 1, Struct{1} }, { 2, Struct{2} }, { 3, Struct{3} } };
	//CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 0; }));
	//CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 1; }));
	//CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 2; }));
	//CHECK(enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 3; }));
	//CHECK(!enumerate::ContainsIf(values, [](auto lhs) { return lhs.m_Value == 4; }));
}

HELPER_TEST_CASE("Difference supports Array.")
{
	const Array<int32> inputA = { 1, 2, 3 };
	const Array<int32> inputB = { 2, 3, 4 };

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

HELPER_TEST_CASE("Difference supports Set.")
{
	const Set<int32> inputA = { 1, 2, 3 };
	const Set<int32> inputB = { 2, 3, 4 };

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

HELPER_TEST_CASE("Difference supports Map.")
{
	//const Map<int32, int32> inputA = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	//const Map<int32, int32> inputB = { { 2, 2 }, { 3, 3 }, { 4, 4 } };

	//INFO("Map only contains values that were present in inputA, but not in inputB.");
	//Map<int32, int32> outputA;
	//enumerate::Difference(inputA, inputB, outputA);
	//CHECK(enumerate::Contains(outputA, 1));
	//CHECK(!enumerate::Contains(outputA, 2));
	//CHECK(!enumerate::Contains(outputA, 3));
	//CHECK(!enumerate::Contains(outputA, 4));

	//INFO("Map only contains values that were present in inputB, but not in inputA.");
	//Map<int32, int32> outputB;
	//enumerate::Difference(inputB, inputA, outputB);
	//CHECK(!enumerate::Contains(outputB, 1));
	//CHECK(!enumerate::Contains(outputB, 2));
	//CHECK(!enumerate::Contains(outputB, 3));
	//CHECK(enumerate::Contains(outputB, 4));
}

HELPER_TEST_CASE("Intersection supports Array.")
{
	const Array<int32> inputA = { 1, 2, 3 };
	const Array<int32> inputB = { 2, 3, 4 };

	Array<int32> output;
	enumerate::Intersection(inputA, inputB, output);
	CHECK(!enumerate::Contains(output, 1));
	CHECK(enumerate::Contains(output, 2));
	CHECK(enumerate::Contains(output, 3));
	CHECK(!enumerate::Contains(output, 4));
}

HELPER_TEST_CASE("Intersection supports Set.")
{
	const Set<int32> inputA = { 1, 2, 3 };
	const Set<int32> inputB = { 2, 3, 4 };

	Set<int32> output;
	enumerate::Intersection(inputA, inputB, output);
	CHECK(!enumerate::Contains(output, 1));
	CHECK(enumerate::Contains(output, 2));
	CHECK(enumerate::Contains(output, 3));
	CHECK(!enumerate::Contains(output, 4));
}

HELPER_TEST_CASE("Intersection supports Map.")
{
	//const Map<int32, int32> inputA = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	//const Map<int32, int32> inputB = { { 2, 2 }, { 3, 3 }, { 4, 4 } };

	//Map<int32, int32> output;
	//enumerate::Intersection(inputA, inputB, output);
	//CHECK(!enumerate::Contains(output, 1));
	//CHECK(enumerate::Contains(output, 2));
	//CHECK(enumerate::Contains(output, 3));
	//CHECK(!enumerate::Contains(output, 4));
}

HELPER_TEST_CASE("BeginForward supports Array.")
{
	const Array<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginForward(input);
	CHECK(*itr == 1);
	itr++;
	CHECK(*itr == 2);
	itr++;
	CHECK(*itr == 3);
}

HELPER_TEST_CASE("BeginForward supports Set.")
{
	const Set<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginForward(input);
	CHECK(*itr == 1);
	itr++;
	CHECK(*itr == 2);
	itr++;
	CHECK(*itr == 3);
}

HELPER_TEST_CASE("BeginForward supports Map.")
{
	const Map<int32, int32> input = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	auto itr = enumerate::BeginForward(input);
	CHECK(itr->first == 1);
	CHECK(itr->second == 1);
	itr++;
	CHECK(itr->first == 2);
	CHECK(itr->second == 2);
	itr++;
	CHECK(itr->first == 3);
	CHECK(itr->second == 3);
}

HELPER_TEST_CASE("BeginReverse supports Array.")
{
	const Array<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginReverse(input);
	CHECK(*itr == 3);
	itr++;
	CHECK(*itr == 2);
	itr++;
	CHECK(*itr == 1);
}

HELPER_TEST_CASE("BeginReverse supports Set.")
{
	const Set<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginReverse(input);
	CHECK(*itr == 3);
	itr++;
	CHECK(*itr == 2);
	itr++;
	CHECK(*itr == 1);
}

HELPER_TEST_CASE("BeginReverse supports Map.")
{
	const Map<int32, int32> input = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	auto itr = enumerate::BeginReverse(input);
	CHECK(itr->first == 3);
	CHECK(itr->second == 3);
	itr++;
	CHECK(itr->first == 2);
	CHECK(itr->second == 2);
	itr++;
	CHECK(itr->first == 1);
	CHECK(itr->second == 1);
}

HELPER_TEST_CASE("EndForward supports Array.")
{
	const Array<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginForward(input);
	auto end = enumerate::EndForward(input);
	itr++; itr++; itr++;
	CHECK(itr == end);
}

HELPER_TEST_CASE("EndForward supports Set.")
{
	const Set<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginForward(input);
	auto end = enumerate::EndForward(input);
	itr++; itr++; itr++;
	CHECK(itr == end);
}

HELPER_TEST_CASE("EndForward supports Map.")
{
	const Map<int32, int32> input = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	auto itr = enumerate::BeginForward(input);
	auto end = enumerate::EndForward(input);
	itr++; itr++; itr++;
	CHECK(itr == end);
}

HELPER_TEST_CASE("EndReverse supports Array.")
{
	const Array<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginReverse(input);
	auto end = enumerate::EndReverse(input);
	itr++; itr++; itr++;
	CHECK(itr == end);
}

HELPER_TEST_CASE("EndReverse supports Set.")
{
	const Set<int32> input = { 1, 2, 3 };
	auto itr = enumerate::BeginReverse(input);
	auto end = enumerate::EndReverse(input);
	itr++; itr++; itr++;
	CHECK(itr == end);
}

HELPER_TEST_CASE("EndReverse supports Map.")
{
	const Map<int32, int32> input = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	auto itr = enumerate::BeginReverse(input);
	auto end = enumerate::EndReverse(input);
	itr++; itr++; itr++;
	CHECK(itr == end);
}

HELPER_TEST_CASE("Forward supports Array.")
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

HELPER_TEST_CASE("Forward supports Set.")
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

HELPER_TEST_CASE("Forward supports Map.")
{
	int32 i = 0;
	Map<int32, int32> values = { { 0, 0 }, { 1, 10 }, { 2, 20 } };
	for (auto&& [index, pair] : enumerate::Forward(values))
	{
		CHECK(index == i);
		CHECK(pair.first == i);
		CHECK(pair.second == i * 10);
		i++;
	}

	INFO("Empty Map doesn't enumerate.");
	values.RemoveAll();
	for (auto&& [index, value] : enumerate::Forward(values))
		FAIL();
}

HELPER_TEST_CASE("Reverse supports Array.")
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

HELPER_TEST_CASE("Reverse supports Set.")
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

HELPER_TEST_CASE("Reverse supports Map.")
{
	int32 i = 0;
	Map<int32, int32> values = { { 0, 0 }, { 1, 10 }, { 2, 20 } };

	auto wrapper = enumerate::Reverse(values);
	auto itr = wrapper.begin();
	CHECK(itr.m_Index == 2);
	CHECK(itr.m_Itr->first == 2);
	CHECK(itr.m_Itr->second == 20);
	++itr;
	CHECK(itr.m_Index == 1);
	CHECK(itr.m_Itr->first == 1);
	CHECK(itr.m_Itr->second == 10);
	++itr;
	CHECK(itr.m_Index == 0);
	CHECK(itr.m_Itr->first == 0);
	CHECK(itr.m_Itr->second == 0);

	INFO("Empty Map doesn't enumerate.");
	values.RemoveAll();
	for (auto&& [index, value] : enumerate::Reverse(values))
		FAIL();
}
