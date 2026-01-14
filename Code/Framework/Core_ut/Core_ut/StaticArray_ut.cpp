#include "CorePCH.h"

#include "Core/StaticArray.h"
#include "Core/Types.h"

#define CLASS_TEST_CASE(name) TEST_CASE("StaticArray. " name, "[StaticArray]")

CLASS_TEST_CASE("Default constructor.")
{
	StaticArray<int32, 3> myArray;
	CHECK(myArray.GetCount() == 3);
}

CLASS_TEST_CASE("Value constructor.")
{
	StaticArray<int32, 3> myArray(3);
	CHECK(myArray.GetCount() == 3);
	CHECK(myArray[0] == 3);
	CHECK(myArray[1] == 3);
	CHECK(myArray[2] == 3);
}

CLASS_TEST_CASE("Move constructor.")
{
	StaticArray<int32, 3> myArrayA = { 1, 2, 3 };
	StaticArray<int32, 3> myArrayB = std::move(myArrayA);
	CHECK(myArrayB.GetCount() == 3);
	CHECK(myArrayB[0] == 1);
	CHECK(myArrayB[1] == 2);
	CHECK(myArrayB[2] == 3);
}

CLASS_TEST_CASE("Copy constructor.")
{
	StaticArray<int32, 3> myArrayA = { 1, 2, 3 };
	StaticArray<int32, 3> myArrayB = myArrayA;
	CHECK(myArrayB.GetCount() == 3);
	CHECK(myArrayB[0] == 1);
	CHECK(myArrayB[1] == 2);
	CHECK(myArrayB[2] == 3);
}

CLASS_TEST_CASE("initializer_list constructor.")
{
	StaticArray<int32, 3> myArray = { 1, 2, 3 };
	CHECK(myArray.GetCount() == 3);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
}

CLASS_TEST_CASE("GetCount.")
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
