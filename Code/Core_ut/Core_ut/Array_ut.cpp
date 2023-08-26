#include <Catch2/catch.hpp>

#include <Core/Array.h>
#include <Core/Slice.h>
#include <Core/Types.h>

namespace
{
	struct MyStruct
	{
		int32 m_Int32 = 0;
	};
}

TEST_CASE("Array")
{
	{
		INFO("Constructors");
		{
			INFO("Default Constructor.");
			Array<int32> myArray;

			CHECK(myArray.GetCount() == 0);
		}

		{
			INFO("Copy Constructor.");
			Array<int32> myArrayA = { 1, 2, 3 };
			Array<int32> myArrayB = myArrayA;

			REQUIRE(myArrayB.GetCount() == 3);
			CHECK(myArrayB[0] == 1);
			CHECK(myArrayB[1] == 2);
			CHECK(myArrayB[2] == 3);
		}

		{
			INFO("Move Constructor.");
			Array<int32> myArrayA = { 1, 2, 3 };
			Array<int32> myArrayB = std::move(myArrayA);

			REQUIRE(myArrayB.GetCount() == 3);
			CHECK(myArrayB[0] == 1);
			CHECK(myArrayB[1] == 2);
			CHECK(myArrayB[2] == 3);
		}

		{
			INFO("std::initializer_list.");
			Array<int32> myArray = { 1, 2, 3 };

			REQUIRE(myArray.GetCount() == 3);
			CHECK(myArray[0] == 1);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 3);
		}
	}

	{
		INFO("operator=");
		Array<int32> myArrayA = { 1, 2, 3 };
		Array<int32> myArrayB = { 4, 5, 6 };
		myArrayA = myArrayB;

		REQUIRE(myArrayA.GetCount() == 3);
		CHECK(myArrayA[0] == 4);
		CHECK(myArrayA[1] == 5);
		CHECK(myArrayA[2] == 6);
	}

	{
		INFO("operator []");
		{
			INFO("Const");
			const Array<int32> myArray = { 1, 2, 3 };

			REQUIRE(myArray.GetCount() == 3);
			CHECK(myArray[0] == 1);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 3);
		}
		{
			INFO("Non-Const");
			Array<int32> myArray = { 1, 2, 3 };
			myArray[0] = 4;
			myArray[1] = 5;
			myArray[2] = 6;

			REQUIRE(myArray.GetCount() == 3);
			CHECK(myArray[0] == 4);
			CHECK(myArray[1] == 5);
			CHECK(myArray[2] == 6);
		}
	}

	{
		INFO("operator Slice");
		{
			INFO("Const");

			Array<int32> myArray = { 1, 2, 3 };
			Slice<int32> mySlice = myArray;
			CHECK(mySlice.GetCount() == 3);
			CHECK(mySlice[0] == 1);
			CHECK(mySlice[1] == 2);
			CHECK(mySlice[2] == 3);
		}

		{
			INFO("Non-Const");

			const Array<int32> myArray = { 1, 2, 3 };
			Slice<const int32> mySlice = myArray;
			CHECK(mySlice.GetCount() == 3);
			CHECK(mySlice[0] == 1);
			CHECK(mySlice[1] == 2);
			CHECK(mySlice[2] == 3);
		}
	}

	SECTION("IsEmpty")
	{
		Array<int32> myArray;
		CHECK(myArray.IsEmpty());

		myArray = { 1, 2, 3 };
		CHECK(!myArray.IsEmpty());
	}

	SECTION("GetCapacity")
	{
		Array<int32> myArray;
		CHECK(myArray.GetCapacity() == 0);

		myArray = { 1, 2, 3 };
		CHECK(myArray.GetCapacity() == 3);

		myArray.Append(1);
		myArray.Append(2);
		myArray.Append(3);
		CHECK(myArray.GetCapacity() == 6);
	}

	SECTION("GetCount")
	{
		Array<int32> myArray;
		CHECK(myArray.GetCount() == 0);

		myArray = { 1, 2, 3 };
		CHECK(myArray.GetCount() == 3);

		myArray.Append(1);
		myArray.Append(2);
		myArray.Append(3);
		CHECK(myArray.GetCount() == 6);
	}

	SECTION("GetData")
	{
	}

	SECTION("Resize")
	{
		Array<int32> myArray;
		CHECK(myArray.GetCount() == 0);

		myArray.Resize(3);
		CHECK(myArray.GetCount() == 3);
	}

	SECTION("Reserve")
	{
		Array<int32> myArray;
		CHECK(myArray.GetCapacity() == 0);

		myArray.Reserve(3);
		CHECK(myArray.GetCapacity() == 3);
	}

	SECTION("GetFirst")
	{
		const Array<int32> myArray = { 1, 2, 3 };
		CHECK(myArray.GetFirst() == 1);
	}

	SECTION("GetLast")
	{
		const Array<int32> myArray = { 1, 2, 3 };
		CHECK(myArray.GetLast() == 3);
	}

	SECTION("Pop")
	{
		Array<int32> myArray = { 1, 2, 3 };
		CHECK(myArray.Pop() == 3);
	}

	SECTION("Append")
	{
		{
			INFO("Copy Value.");
			MyStruct myStruct = { 1 };
			Array<MyStruct> myArray;
			myArray.Append(myStruct);

			REQUIRE(myArray.GetCount() == 1);
			CHECK(myArray[0].m_Int32 == 1);
		}

		{
			INFO("Move Value.");
			MyStruct myStruct = { 1 };
			Array<MyStruct> myArray;
			myArray.Append(std::move(myStruct));

			REQUIRE(myArray.GetCount() == 1);
			CHECK(myArray[0].m_Int32 == 1);
		}

		{
			INFO("Copy Range.");
			Array<int32> myArrayA;
			Array<int32> myArrayB = { 3, 4, 5 };
			myArrayA.Append(myArrayB);

			REQUIRE(myArrayA.GetCount() == 3);
			CHECK(myArrayA[0] == 3);
			CHECK(myArrayA[1] == 4);
			CHECK(myArrayA[2] == 5);
		}

		{
			INFO("Move Range.");
			Array<int32> myArrayA;
			Array<int32> myArrayB = { 3, 4, 5 };
			myArrayA.Append(std::move(myArrayB));

			REQUIRE(myArrayA.GetCount() == 3);
			CHECK(myArrayA[0] == 3);
			CHECK(myArrayA[1] == 4);
			CHECK(myArrayA[2] == 5);
		}

		{
			INFO("std::initializer_list.");
			Array<int32> myArray;
			myArray.Append({ 3, 4, 5 });

			REQUIRE(myArray.GetCount() == 3);
			CHECK(myArray[0] == 3);
			CHECK(myArray[1] == 4);
			CHECK(myArray[2] == 5);
		}
	}

	SECTION("Emplace")
	{
		Array<MyStruct> myArray;
		myArray.Emplace(1);

		REQUIRE(myArray.GetCount() == 1);
		CHECK(myArray[0].m_Int32 == 1);
	}

	SECTION("Insert")
	{
		{
			INFO("Copy Value.");
			MyStruct myStructA = { 1 };
			MyStruct myStructB = { 2 };
			MyStruct myStructC = { 3 };
			Array<MyStruct> myArray;
			myArray.Insert(myStructA, 0);
			myArray.Insert(myStructB, 0);
			myArray.Insert(myStructC, 0);

			REQUIRE(myArray.GetCount() == 3);
			CHECK(myArray[0].m_Int32 == 3);
			CHECK(myArray[1].m_Int32 == 2);
			CHECK(myArray[2].m_Int32 == 1);
		}

		{
			INFO("Move Value.");
			MyStruct myStructA = { 1 };
			MyStruct myStructB = { 2 };
			MyStruct myStructC = { 3 };
			Array<MyStruct> myArray;
			myArray.Insert(std::move(myStructA), 0);
			myArray.Insert(std::move(myStructB), 0);
			myArray.Insert(std::move(myStructC), 0);

			REQUIRE(myArray.GetCount() == 3);
			CHECK(myArray[0].m_Int32 == 3);
			CHECK(myArray[1].m_Int32 == 2);
			CHECK(myArray[2].m_Int32 == 1);
		}
	}

	SECTION("RemoveAt")
	{
		{
			INFO("First");

			Array<int32> myArray = { 1, 2, 3, 4, 5 };
			myArray.RemoveAt(0);

			REQUIRE(myArray.GetCount() == 4);
			CHECK(myArray[0] == 5);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 3);
			CHECK(myArray[3] == 4);
		}

		{
			INFO("Middle");

			Array<int32> myArray = { 1, 2, 3, 4 ,5 };
			myArray.RemoveAt(2);

			REQUIRE(myArray.GetCount() == 4);
			CHECK(myArray[0] == 1);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 5);
			CHECK(myArray[3] == 4);
		}

		{
			INFO("Last");

			Array<int32> myArray = { 1, 2, 3, 4 ,5 };
			myArray.RemoveAt(4);

			REQUIRE(myArray.GetCount() == 4);
			CHECK(myArray[0] == 1);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 3);
			CHECK(myArray[3] == 4);
		}
	}

	SECTION("RemoveOrderedAt")
	{
		{
			INFO("First");

			Array<int32> myArray = { 1, 2, 3, 4, 5 };
			myArray.RemoveOrderedAt(0);

			REQUIRE(myArray.GetCount() == 4);
			CHECK(myArray[0] == 2);
			CHECK(myArray[1] == 3);
			CHECK(myArray[2] == 4);
			CHECK(myArray[3] == 5);
		}

		{
			INFO("Middle");

			Array<int32> myArray = { 1, 2, 3, 4 ,5 };
			myArray.RemoveOrderedAt(2);

			REQUIRE(myArray.GetCount() == 4);
			CHECK(myArray[0] == 1);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 4);
			CHECK(myArray[3] == 5);
		}

		{
			INFO("Last");

			Array<int32> myArray = { 1, 2, 3, 4 ,5 };
			myArray.RemoveOrderedAt(4);

			REQUIRE(myArray.GetCount() == 4);
			CHECK(myArray[0] == 1);
			CHECK(myArray[1] == 2);
			CHECK(myArray[2] == 3);
			CHECK(myArray[3] == 4);
		}
	}

	SECTION("RemoveLast")
	{
		Array<int32> myArray = { 1, 2, 3 };
		myArray.RemoveLast();

		REQUIRE(myArray.GetCount() == 2);
		CHECK(myArray[0] == 1);
		CHECK(myArray[1] == 2);
	}

	SECTION("RemoveAll")
	{
		Array<int32> myArray = { 1, 2, 3 };
		myArray.RemoveAll();

		CHECK(myArray.GetCount() == 0);
	}

	SECTION("Iterator")
	{
		Array<int32> myArray = { 1, 2, 3 };

		{
			INFO("Pre-increment.");

			auto iterator = myArray.begin();
			CHECK(*iterator == 1);
			CHECK(*++iterator == 2);
			CHECK(*++iterator == 3);
			CHECK(++iterator == myArray.end());
		}

		{
			INFO("Post-increment.");

			auto iterator = myArray.begin();
			CHECK(*iterator++ == 1);
			CHECK(*iterator++ == 2);
			CHECK(*iterator++ == 3);
			CHECK(iterator == myArray.end());
		}

		SECTION("Range-based for loop")
		{
			int32 i = -1;
			int32 checks[] = { 1, 2, 3 };
			for (int32& value : myArray)
			{
				i++;
				CHECK(value == checks[i]);

				value = 1337;
			}

			// check that the values were modified
			for (int32& value : myArray)
				CHECK(value == 1337);
		}
	}
}