#include <Catch2/catch.hpp>

#include "Core/Array.h"
#include "Core/Slice.h"
#include "Core/Types.h"

TEST_CASE("Slice. Constructors.")
{
	{
		INFO("Default Constructor");
		Slice<int32> mySlice;

		CHECK(mySlice.GetCount() == 0);
	}

	{
		INFO("Type*, Count");
		Array<int32> myArray = { 1, 2, 3 };
		Slice<int32> mySlice = Slice<int32>(myArray.GetData(), myArray.GetCount());

		REQUIRE(mySlice.GetCount() == 3);
		CHECK(mySlice[0] == 1);
		CHECK(mySlice[1] == 2);
		CHECK(mySlice[2] == 3);
	}

	{
		INFO("Type*, Type*");
		Array<int32> myArray = { 1, 2, 3, 4 };
		Slice<int32> mySlice = Slice<int32>(&myArray[1], &myArray[3]);

		REQUIRE(mySlice.GetCount() == 2);
		CHECK(mySlice[0] == 2);
		CHECK(mySlice[1] == 3);
	}

	{
		INFO("Slice");
		Array<int32> myArray = { 1, 2, 3 };
		Slice<int32> mySliceA = myArray;
		Slice<int32> mySliceB = mySliceA;

		REQUIRE(mySliceB.GetCount() == 3);
		CHECK(mySliceB[0] == 1);
		CHECK(mySliceB[1] == 2);
		CHECK(mySliceB[2] == 3);
	}
}

TEST_CASE("Slice. operator=.")
{
	{
		INFO("Slice");
		Array<int32> myArray = { 1, 2, 3 };
		Slice<int32> mySliceA = myArray;
		Slice<int32> mySliceB;

		CHECK(mySliceB.IsNull());
		mySliceB = mySliceA;

		REQUIRE(mySliceB.GetCount() == 3);
		CHECK(mySliceB[0] == 1);
		CHECK(mySliceB[1] == 2);
		CHECK(mySliceB[2] == 3);
	}
}

TEST_CASE("Slice. operator[].")
{
	{
		INFO("Slice<Type>");
		Array<int32> myArray = { 1, 2, 3 };
		Slice<int32> mySlice = myArray;

		REQUIRE(myArray.GetCount() == 3);
		CHECK(myArray[0] == 1);
		CHECK(myArray[1] == 2);
		CHECK(myArray[2] == 3);

		REQUIRE(mySlice.GetCount() == 3);
		CHECK(mySlice[0] == 1);
		CHECK(mySlice[1] == 2);
		CHECK(mySlice[2] == 3);

		mySlice[0] = 4;
		mySlice[1] = 5;
		mySlice[2] = 6;

		REQUIRE(myArray.GetCount() == 3);
		CHECK(myArray[0] == 4);
		CHECK(myArray[1] == 5);
		CHECK(myArray[2] == 6);

		REQUIRE(mySlice.GetCount() == 3);
		CHECK(mySlice[0] == 4);
		CHECK(mySlice[1] == 5);
		CHECK(mySlice[2] == 6);
	}

	{
		INFO("Slice<const Type>");
		const Array<int32> myArray = { 1, 2, 3 };
		Slice<const int32> mySlice = myArray;

		REQUIRE(mySlice.GetCount() == 3);
		CHECK(mySlice[0] == 1);
		CHECK(mySlice[1] == 2);
		CHECK(mySlice[2] == 3);
	}

	{
		INFO("const Slice<Type>");
		Array<int32> myArray = { 1, 2, 3 };
		const Slice<int32> mySlice = myArray;

		REQUIRE(mySlice.GetCount() == 3);
		CHECK(mySlice[0] == 1);
		CHECK(mySlice[1] == 2);
		CHECK(mySlice[2] == 3);
	}
}

TEST_CASE("Slice. operator Slice<const Type>.")
{
	Array<int32> myArray = { 1, 2, 3 };
	Slice<int32> mySliceA = myArray;
	Slice<const int32> mySliceB = mySliceA;

	CHECK(mySliceB.GetCount() == 3);
}

TEST_CASE("Slice. IsEmpty.")
{
	Slice<int32> mySlice;
	CHECK(mySlice.IsEmpty());

	Array<int32> myArray = { 1, 2, 3 };
	mySlice = myArray;
	CHECK(!mySlice.IsEmpty());

	myArray.RemoveAll();

	// slice doesn't change even if the memory was changed
	CHECK(!mySlice.IsEmpty());
}

TEST_CASE("Slice. IsNull.")
{
	Slice<int32> mySlice;
	CHECK(mySlice.IsNull());

	Array<int32> myArray = { 1, 2, 3 };
	mySlice = myArray;
	CHECK(!mySlice.IsNull());

	myArray.RemoveAll();

	CHECK(!mySlice.IsNull());
}

TEST_CASE("Slice. GetCount.")
{
	Slice<int32> mySlice;
	CHECK(mySlice.GetCount() == 0);

	Array<int32> myArray = { 1, 2, 3 };
	mySlice = myArray;
	CHECK(mySlice.GetCount() == 3);

	myArray.RemoveAll();

	// slice doesn't change even if the memory was changed
	CHECK(mySlice.GetCount() == 3);
}
