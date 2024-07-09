#include <Catch2/catch.hpp>

#include "Core/Set.h"
#include "Core/Types.h"

namespace
{
	struct MyStruct
	{
		bool operator<(const MyStruct& rhs) const { return m_Int32 < rhs.m_Int32; }
		int32 m_Int32 = 0;
	};
}

TEST_CASE("Set")
{
	SECTION("Constructors")
	{
		{
			INFO("Default Constructor.");
			Set<int32> mySet;

			CHECK(mySet.GetCount() == 0);
		}

		{
			INFO("Copy Constructor.");
			Set<int32> mySetA = { 1, 2, 3 };
			Set<int32> mySetB = mySetA;

			CHECK(mySetB.GetCount() == 3);
			CHECK(mySetB.Contains(1));
			CHECK(mySetB.Contains(2));
			CHECK(mySetB.Contains(3));
		}

		{
			INFO("Move Constructor.");
			Set<int32> mySetA = { 1, 2, 3 };
			Set<int32> mySetB = std::move(mySetA);

			CHECK(mySetB.GetCount() == 3);
			CHECK(mySetB.Contains(1));
			CHECK(mySetB.Contains(2));
			CHECK(mySetB.Contains(3));
		}

		{
			INFO("std::initializer_list.");
			Set<int32> mySet = { 1, 2, 3 };

			CHECK(mySet.GetCount() == 3);
			CHECK(mySet.Contains(1));
			CHECK(mySet.Contains(2));
			CHECK(mySet.Contains(3));
		}
	}

	SECTION("operator=")
	{
		{
			INFO("Copy Operator.");
			Set<int32> mySetA = { 1, 2, 3 };
			Set<int32> mySetB;
			mySetB = mySetA;

			CHECK(mySetB.GetCount() == 3);
			CHECK(mySetB.Contains(1));
			CHECK(mySetB.Contains(2));
			CHECK(mySetB.Contains(3));
		}

		{
			INFO("Move Operator.");
			Set<int32> mySetA = { 1, 2, 3 };
			Set<int32> mySetB;
			mySetB = std::move(mySetA);

			CHECK(mySetB.GetCount() == 3);
			CHECK(mySetB.Contains(1));
			CHECK(mySetB.Contains(2));
			CHECK(mySetB.Contains(3));
		}

		{
			INFO("std::initializer_list.");
			Set<int32> mySet;
			mySet = { 1, 2, 3 };

			CHECK(mySet.GetCount() == 3);
			CHECK(mySet.Contains(1));
			CHECK(mySet.Contains(2));
			CHECK(mySet.Contains(3));
		}
	}

	SECTION("IsEmpty")
	{
		Set<int32> mySet;
		CHECK(mySet.IsEmpty());

		mySet = { 1, 2, 3 };
		CHECK(!mySet.IsEmpty());
	}

	SECTION("GetCount")
	{
		Set<int32> mySet;
		CHECK(mySet.GetCount() == 0);

		mySet = { 1, 2, 3 };
		CHECK(mySet.GetCount() == 3);

		mySet.Add(1);
		mySet.Add(2);
		mySet.Add(3);
		CHECK(mySet.GetCount() == 3);
	}

	SECTION("Contains")
	{
		Set<int32> mySet = { 1, 3 };

		CHECK(mySet.Contains(1));
		CHECK(!mySet.Contains(2));
	}

	SECTION("Add")
	{
		{
			INFO("Copy Value.");
			Set<int32> mySet;
			mySet.Add(1);

			CHECK(mySet.GetCount() == 1);
			CHECK(mySet.Contains(1));
		}

		{
			INFO("Move Value.");
			MyStruct myStruct = { 1 };
			Set<MyStruct> mySet;
			mySet.Add(std::move(myStruct));

			CHECK(mySet.GetCount() == 1);
			CHECK(mySet.Contains({ 1 }));
		}

		{
			INFO("Copy Range.");
			Set<int32> mySetA;
			Set<int32> mySetB = { 3, 4, 5 };
			mySetA.Add(mySetB);

			CHECK(mySetA.GetCount() == 3);
			CHECK(mySetA.Contains(3));
			CHECK(mySetA.Contains(4));
			CHECK(mySetA.Contains(5));
		}

		{
			INFO("Move Range.");
			Set<int32> mySetA;
			Set<int32> mySetB = { 3, 4, 5 };
			mySetA.Add(std::move(mySetB));

			CHECK(mySetA.GetCount() == 3);
			CHECK(mySetA.Contains(3));
			CHECK(mySetA.Contains(4));
			CHECK(mySetA.Contains(5));
		}
	}

	SECTION("Remove")
	{
		{
			INFO("Value.");
			Set<int32> mySet = { 1, 2, 3 };
			mySet.Remove(2);

			CHECK(mySet.GetCount() == 2);
			CHECK(mySet.Contains(1));
			CHECK(!mySet.Contains(2));
			CHECK(mySet.Contains(3));
		}

		{
			INFO("Range.");
			Set<int32> mySetA = { 1, 2, 3 };
			Set<int32> mySetB = { 1, 4 };
			mySetA.Remove(mySetB);

			CHECK(mySetA.GetCount() == 2);
			CHECK(!mySetA.Contains(1));
			CHECK(mySetA.Contains(2));
			CHECK(mySetA.Contains(3));
		}
	}

	SECTION("RemoveAll")
	{
		Set<int32> mySet = { 1, 2, 3 };
		mySet.RemoveAll();

		CHECK(mySet.GetCount() == 0);
		CHECK(!mySet.Contains(1));
		CHECK(!mySet.Contains(2));
		CHECK(!mySet.Contains(3));
	}

	SECTION("Range-based for loop")
	{
		Set<int32> mySet = { 1, 2, 3 };

		{
			INFO("Pre-increment.");

			auto iterator = mySet.begin();
			CHECK(*iterator == 1);
			CHECK(*++iterator == 2);
			CHECK(*++iterator == 3);
			CHECK(++iterator == mySet.end());
		}

		{
			INFO("Post-increment.");

			auto iterator = mySet.begin();
			CHECK(*iterator++ == 1);
			CHECK(*iterator++ == 2);
			CHECK(*iterator++ == 3);
			CHECK(iterator == mySet.end());
		}

		SECTION("Range-based for loop")
		{
			int32 i = -1;
			int32 checks[] = { 1, 2, 3 };
			for (int32 value : mySet)
			{
				i++;
				CHECK(value == checks[i]);
			}
		}
	}
}