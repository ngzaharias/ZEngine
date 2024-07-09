#include <Catch2/catch.hpp>

#include "Core/Map.h"
#include "Core/Types.h"

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

TEST_CASE("Map")
{
	SECTION("Constructors")
	{
		{
			INFO("Default Constructor.");
			Map<int32, int32> myMap;

			CHECK(myMap.GetCount() == 0);
		}

		{
			INFO("Copy Constructor.");
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

		{
			INFO("Move Constructor.");
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

		{
			INFO("std::initializer_list.");
			Map<int32, int32> myMap = { { 1, 1 }, { 2, 2 }, { 3, 3 } };

			CHECK(myMap.GetCount() == 3);
			CHECK(myMap.Contains(1));
			CHECK(myMap.Contains(2));
			CHECK(myMap.Contains(3));
			CHECK(myMap[1] == 1);
			CHECK(myMap[2] == 2);
			CHECK(myMap[3] == 3);
		}
	}

	SECTION("operator[]")
	{
	}

	SECTION("operator=")
	{
	}

	SECTION("IsEmpty")
	{
		Map<int32, int32> myMap;
		CHECK(myMap.IsEmpty());

		myMap = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
		CHECK(!myMap.IsEmpty());
	}

	SECTION("GetCount")
	{
		Map<int32, int32> myMap;
		CHECK(myMap.GetCount() == 0);

		myMap = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
		CHECK(myMap.GetCount() == 3);
	}

	SECTION("Contains")
	{
		Map<int32, int32> myMap = { { 1, 1 }, { 3, 3 } };

		CHECK(myMap.Contains(1));
		CHECK(!myMap.Contains(2));
	}

	SECTION("Find")
	{
	}

	SECTION("Emplace")
	{
		{
			Map<int32, MyStruct> myMap;
			myMap.Emplace(1, 1);

			CHECK(myMap.GetCount() == 1);
			CHECK(myMap.Contains(1));
			CHECK(myMap[1].m_Int32 == 1);
		}

		{
			INFO("Emplace with the same key won't change the value.");
			Map<int32, MyStruct> myMap;
			myMap.Emplace(1, 1);
			myMap.Emplace(1, 2);

			CHECK(myMap.GetCount() == 1);
			CHECK(myMap[1].m_Int32 == 1);
		}
	}

	SECTION("Insert")
	{
	}

	SECTION("Set")
	{
	}

	SECTION("Remove")
	{
	}

	SECTION("RemoveAll")
	{
	}

	SECTION("Range-based for loop")
	{
	}
}