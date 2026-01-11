#include "CorePCH.h"

#include "Core/Map.h"
#include "Core/Types.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Map. " name, "[Map]")

namespace
{
	struct MyStruct
	{
		MyStruct() = default;
		MyStruct(int32 value) : m_Int32(value) { }
		bool operator<(const MyStruct& rhs) const { return m_Int32 < rhs.m_Int32; }
		int32 m_Int32 = 0;
	};
}

CLASS_TEST_CASE("Default constructor.")
{
	Map<int32, int32> myMap;

	CHECK(myMap.GetCount() == 0);
}

CLASS_TEST_CASE("Copy constructor.")
{
	Map<int32, int32> myMapA = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	Map<int32, int32> myMapB = myMapA;

	CHECK(myMapB.GetCount() == 3);
	CHECK(myMapB.Contains(1));
	CHECK(myMapB.Contains(2));
	CHECK(myMapB.Contains(3));
	CHECK(myMapB[1] == 1);
	CHECK(myMapB[2] == 2);
	CHECK(myMapB[3] == 3);
}

CLASS_TEST_CASE("Move constructor.")
{
	Map<int32, int32> myMapA = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	Map<int32, int32> myMapB = std::move(myMapA);

	CHECK(myMapB.GetCount() == 3);
	CHECK(myMapB.Contains(1));
	CHECK(myMapB.Contains(2));
	CHECK(myMapB.Contains(3));
	CHECK(myMapB[1] == 1);
	CHECK(myMapB[2] == 2);
	CHECK(myMapB[3] == 3);
}

CLASS_TEST_CASE("initializer_list constructor.")
{
	Map<int32, int32> myMap = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

	CHECK(myMap.GetCount() == 3);
	CHECK(myMap.Contains(1));
	CHECK(myMap.Contains(2));
	CHECK(myMap.Contains(3));
	CHECK(myMap[1] == 1);
	CHECK(myMap[2] == 2);
	CHECK(myMap[3] == 3);
}

CLASS_TEST_CASE("IsEmpty")
{
	Map<int32, int32> myMap;
	CHECK(myMap.IsEmpty());

	myMap = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	CHECK(!myMap.IsEmpty());
}

CLASS_TEST_CASE("GetCount")
{
	Map<int32, int32> myMap;
	CHECK(myMap.GetCount() == 0);

	myMap = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	CHECK(myMap.GetCount() == 3);
}

CLASS_TEST_CASE("Contains")
{
	Map<int32, int32> myMap = { { 1, 1 }, { 3, 3 } };

	CHECK(myMap.Contains(1));
	CHECK(!myMap.Contains(2));
}

CLASS_TEST_CASE("Emplace.")
{
	Map<int32, MyStruct> myMap;
	myMap.Emplace(1, 1);

	CHECK(myMap.GetCount() == 1);
	CHECK(myMap.Contains(1));
	CHECK(myMap[1].m_Int32 == 1);
}

CLASS_TEST_CASE("Emplace with the same key won't change the value.")
{
	Map<int32, MyStruct> myMap;
	myMap.Emplace(1, 1);
	myMap.Emplace(1, 2);

	CHECK(myMap.GetCount() == 1);
	CHECK(myMap[1].m_Int32 == 1);
}
