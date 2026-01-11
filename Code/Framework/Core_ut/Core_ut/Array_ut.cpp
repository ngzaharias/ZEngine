#include "CorePCH.h"

#include "Core/Array.h"
#include "Core/Slice.h"
#include "Core/Types.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Array. " name, "[Array]")

namespace
{
	struct MyStruct
	{
		int32 m_Int32 = 0;
	};
}

CLASS_TEST_CASE("Default constructor.")
{
	Array<int32> myArray;

	CHECK(myArray.GetCount() == 0);
}

CLASS_TEST_CASE("Copy constructor.")
{
	Array<int32> myArrayA = { 1, 2, 3 };
	Array<int32> myArrayB = myArrayA;

	REQUIRE(myArrayB.GetCount() == 3);
	CHECK(myArrayB[0] == 1);
	CHECK(myArrayB[1] == 2);
	CHECK(myArrayB[2] == 3);
}

CLASS_TEST_CASE("Move constructor.")
{
	Array<int32> myArrayA = { 1, 2, 3 };
	Array<int32> myArrayB = std::move(myArrayA);

	REQUIRE(myArrayB.GetCount() == 3);
	CHECK(myArrayB[0] == 1);
	CHECK(myArrayB[1] == 2);
	CHECK(myArrayB[2] == 3);
}

CLASS_TEST_CASE("initializer_list constructor.")
{
	Array<int32> myArray = { 1, 2, 3 };

	REQUIRE(myArray.GetCount() == 3);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
}

CLASS_TEST_CASE("operator = .")
{
	Array<int32> myArrayA = { 1, 2, 3 };
	Array<int32> myArrayB = { 4, 5, 6 };
	myArrayA = myArrayB;

	REQUIRE(myArrayA.GetCount() == 3);
	CHECK(myArrayA[0] == 4);
	CHECK(myArrayA[1] == 5);
	CHECK(myArrayA[2] == 6);
}

CLASS_TEST_CASE("operator[].")
{
	Array<int32> myArray = { 1, 2, 3 };
	myArray[0] = 4;
	myArray[1] = 5;
	myArray[2] = 6;

	REQUIRE(myArray.GetCount() == 3);
	CHECK(myArray[0] == 4);
	CHECK(myArray[1] == 5);
	CHECK(myArray[2] == 6);
}

CLASS_TEST_CASE("operator[] const.")
{
	const Array<int32> myArray = { 1, 2, 3 };

	REQUIRE(myArray.GetCount() == 3);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
}

CLASS_TEST_CASE("operator Slice<T>.")
{
	const Array<int32> myArray = { 1, 2, 3 };
	Slice<const int32> mySlice = myArray;
	CHECK(mySlice.GetCount() == 3);
	CHECK(mySlice[0] == 1);
	CHECK(mySlice[1] == 2);
	CHECK(mySlice[2] == 3);
}

CLASS_TEST_CASE("operator Slice<T> const.")
{
	Array<int32> myArray = { 1, 2, 3 };
	Slice<int32> mySlice = myArray;
	CHECK(mySlice.GetCount() == 3);
	CHECK(mySlice[0] == 1);
	CHECK(mySlice[1] == 2);
	CHECK(mySlice[2] == 3);
}

CLASS_TEST_CASE("IsEmpty.")
{
	Array<int32> myArray;
	CHECK(myArray.IsEmpty());

	myArray = { 1, 2, 3 };
	CHECK(!myArray.IsEmpty());
}

CLASS_TEST_CASE("GetCapacity.")
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

CLASS_TEST_CASE("GetCount.")
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

CLASS_TEST_CASE("GetData.")
{
	Array<int32> myArray = { 0, 1, 2 };
	CHECK(myArray.GetData()[0] == 0);
	CHECK(myArray.GetData()[1] == 1);
	CHECK(myArray.GetData()[2] == 2);
}

CLASS_TEST_CASE("Resize.")
{
	Array<int32> myArray;
	CHECK(myArray.GetCount() == 0);

	myArray.Resize(3);
	CHECK(myArray.GetCount() == 3);
}

CLASS_TEST_CASE("Reserve.")
{
	Array<int32> myArray;
	CHECK(myArray.GetCapacity() == 0);

	myArray.Reserve(3);
	CHECK(myArray.GetCapacity() == 3);
}

CLASS_TEST_CASE("GetFirst.")
{
	const Array<int32> myArray = { 1, 2, 3 };
	CHECK(myArray.GetFirst() == 1);
}

CLASS_TEST_CASE("GetLast.")
{
	const Array<int32> myArray = { 1, 2, 3 };
	CHECK(myArray.GetLast() == 3);
}

CLASS_TEST_CASE("Pop.")
{
	Array<int32> myArray = { 1, 2, 3 };
	CHECK(myArray.Pop() == 3);
}

CLASS_TEST_CASE("Append copy value.")
{
	MyStruct myStruct = { 1 };
	Array<MyStruct> myArray;
	myArray.Append(myStruct);

	REQUIRE(myArray.GetCount() == 1);
	CHECK(myArray[0].m_Int32 == 1);
}

CLASS_TEST_CASE("Append move value.")
{
	MyStruct myStruct = { 1 };
	Array<MyStruct> myArray;
	myArray.Append(std::move(myStruct));

	REQUIRE(myArray.GetCount() == 1);
	CHECK(myArray[0].m_Int32 == 1);
}

CLASS_TEST_CASE("Append copy range.")
{
	Array<int32> myArrayA;
	Array<int32> myArrayB = { 3, 4, 5 };
	myArrayA.Append(myArrayB);

	REQUIRE(myArrayA.GetCount() == 3);
	CHECK(myArrayA[0] == 3);
	CHECK(myArrayA[1] == 4);
	CHECK(myArrayA[2] == 5);
}

CLASS_TEST_CASE("Append move range.")
{
	Array<int32> myArrayA;
	Array<int32> myArrayB = { 3, 4, 5 };
	myArrayA.Append(std::move(myArrayB));

	REQUIRE(myArrayA.GetCount() == 3);
	CHECK(myArrayA[0] == 3);
	CHECK(myArrayA[1] == 4);
	CHECK(myArrayA[2] == 5);
}

CLASS_TEST_CASE("Append initializer_list.")
{
	Array<int32> myArray;
	myArray.Append({ 3, 4, 5 });

	REQUIRE(myArray.GetCount() == 3);
	CHECK(myArray[0] == 3);
	CHECK(myArray[1] == 4);
	CHECK(myArray[2] == 5);
}

CLASS_TEST_CASE("Emplace")
{
	Array<MyStruct> myArray;
	myArray.Emplace(1);

	REQUIRE(myArray.GetCount() == 1);
	CHECK(myArray[0].m_Int32 == 1);
}

CLASS_TEST_CASE("Insert copy value.")
{
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

CLASS_TEST_CASE("Insert move value.")
{
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

CLASS_TEST_CASE("RemoveAt first by index.")
{
	Array<int32> myArray = { 1, 2, 3, 4, 5 };
	myArray.RemoveAt(0);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 5);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveAt middle by index.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveAt(2);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 5);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveAt last by index.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveAt(4);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveAt first by iterator.")
{
	Array<int32> myArray = { 1, 2, 3, 4, 5 };
	myArray.RemoveAt(myArray.begin());

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 5);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveAt middle by iterator.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveAt(myArray.begin() + 2);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 5);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveAt last by iterator.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveAt(myArray.begin() + 4);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveOrderedAt first by index.")
{
	Array<int32> myArray = { 1, 2, 3, 4, 5 };
	myArray.RemoveOrderedAt(0);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 2);
	CHECK(myArray[1] == 3);
	CHECK(myArray[2] == 4);
	CHECK(myArray[3] == 5);
}

CLASS_TEST_CASE("RemoveOrderedAt middle by index.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveOrderedAt(2);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 4);
	CHECK(myArray[3] == 5);
}

CLASS_TEST_CASE("RemoveOrderedAt last by index.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveOrderedAt(4);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveOrderedAt first by iterator.")
{
	Array<int32> myArray = { 1, 2, 3, 4, 5 };
	myArray.RemoveOrderedAt(myArray.begin());

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 2);
	CHECK(myArray[1] == 3);
	CHECK(myArray[2] == 4);
	CHECK(myArray[3] == 5);
}

CLASS_TEST_CASE("RemoveOrderedAt middle by iterator.")
{
	Array<int32> myArray = { 1, 2, 3, 4, 5 };
	myArray.RemoveOrderedAt(myArray.begin() + 2);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 4);
	CHECK(myArray[3] == 5);
}

CLASS_TEST_CASE("RemoveOrderedAt last by iterator.")
{
	Array<int32> myArray = { 1, 2, 3, 4 ,5 };
	myArray.RemoveOrderedAt(myArray.begin() + 4);

	REQUIRE(myArray.GetCount() == 4);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
	CHECK(myArray[2] == 3);
	CHECK(myArray[3] == 4);
}

CLASS_TEST_CASE("RemoveLast.")
{
	Array<int32> myArray = { 1, 2, 3 };
	myArray.RemoveLast();

	REQUIRE(myArray.GetCount() == 2);
	CHECK(myArray[0] == 1);
	CHECK(myArray[1] == 2);
}

CLASS_TEST_CASE("RemoveAll.")
{
	Array<int32> myArray = { 1, 2, 3 };
	myArray.RemoveAll();

	CHECK(myArray.GetCount() == 0);
}

CLASS_TEST_CASE("Iterator pre - increment.")
{
	Array<int32> myArray = { 1, 2, 3 };

	auto iterator = myArray.begin();
	CHECK(*iterator == 1);
	CHECK(*++iterator == 2);
	CHECK(*++iterator == 3);
	CHECK(++iterator == myArray.end());
}

CLASS_TEST_CASE("Iterator post - increment.")
{
	Array<int32> myArray = { 1, 2, 3 };

	auto iterator = myArray.begin();
	CHECK(*iterator++ == 1);
	CHECK(*iterator++ == 2);
	CHECK(*iterator++ == 3);
	CHECK(iterator == myArray.end());
}

CLASS_TEST_CASE("Iterator range-based for loop.")
{
	Array<int32> myArray = { 1, 2, 3 };

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