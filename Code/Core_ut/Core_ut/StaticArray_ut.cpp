#include <Catch2/catch.hpp>

#include "Core/StaticArray.h"
#include "Core/Types.h"

TEST_CASE("StaticArray. Default constructor.")
{
	StaticArray<int32, 3> myArray;
	CHECK(myArray.GetCount() == 3);
}

TEST_CASE("StaticArray. Value constructor.")
{
	StaticArray<int32, 3> myArray(3);
	CHECK(myArray.GetCount() == 3);
	CHECK(myArray[0] == 3);
	CHECK(myArray[1] == 3);
	CHECK(myArray[2] == 3);
}

TEST_CASE("StaticArray. Move constructor.")
{
	StaticArray<int32, 3> myArrayA = { 1, 2, 3 };
	StaticArray<int32, 3> myArrayB = std::move(myArrayA);
	CHECK(myArrayB.GetCount() == 3);
	CHECK(myArrayB[0] == 1);
	CHECK(myArrayB[1] == 2);
	CHECK(myArrayB[2] == 3);
}

TEST_CASE("StaticArray. Copy constructor.")
{
	StaticArray<int32, 3> myArrayA = { 1, 2, 3 };
	StaticArray<int32, 3> myArrayB = myArrayA;
	CHECK(myArrayB.GetCount() == 3);
	CHECK(myArrayB[0] == 1);
	CHECK(myArrayB[1] == 2);
	CHECK(myArrayB[2] == 3);
}

TEST_CASE("StaticArray. initializer_list constructor.")
{
	StaticArray<int32, 3> myArray = { 1, 2, 3 };
	CHECK(myArray.GetCount() == 3);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
}

TEST_CASE("StaticArray. GetCount.")
{
	constexpr int32 big = 500'000'000;
	StaticArray<int32, 3> myArrayA;
	StaticArray<int32, 5> myArrayB;
	StaticArray<int32, 999> myArrayC;
	StaticArray<int32, big>* myArrayD = new StaticArray<int32, big>();
	CHECK(myArrayA.GetCount() == 3);
	CHECK(myArrayB.GetCount() == 5);
	CHECK(myArrayC.GetCount() == 999);
	CHECK(myArrayD->GetCount() == big);
}
