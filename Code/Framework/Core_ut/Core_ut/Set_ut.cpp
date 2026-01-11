#include "CorePCH.h"

#include "Core/Set.h"
#include "Core/Types.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Set. " name, "[Set]")

namespace
{
	struct MyStruct
	{
		bool operator<(const MyStruct& rhs) const { return m_Int32 < rhs.m_Int32; }
		int32 m_Int32 = 0;
	};
}

CLASS_TEST_CASE("Default constructor.")
{
	Set<int32> mySet;

	CHECK(mySet.GetCount() == 0);
}

CLASS_TEST_CASE("Copy constructor.")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB = mySetA;

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

CLASS_TEST_CASE("Move constructor.")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB = std::move(mySetA);

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

CLASS_TEST_CASE("initializer_list constructor.")
{
	Set<int32> mySet = { 1, 2, 3 };

	CHECK(mySet.GetCount() == 3);
	CHECK(mySet.Contains(1));
	CHECK(mySet.Contains(2));
	CHECK(mySet.Contains(3));
}

CLASS_TEST_CASE("operator=(Set&&).")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB;
	mySetB = std::move(mySetA);

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

CLASS_TEST_CASE("operator=(const Set&).")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB;
	mySetB = mySetA;

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

CLASS_TEST_CASE("operator=(initializer_list&&).")
{
	Set<int32> mySet;
	mySet = { 1, 2, 3 };

	CHECK(mySet.GetCount() == 3);
	CHECK(mySet.Contains(1));
	CHECK(mySet.Contains(2));
	CHECK(mySet.Contains(3));
}

CLASS_TEST_CASE("IsEmpty.")
{
	Set<int32> mySet;
	CHECK(mySet.IsEmpty());

	mySet = { 1, 2, 3 };
	CHECK(!mySet.IsEmpty());
}

CLASS_TEST_CASE("GetCount.")
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

CLASS_TEST_CASE("Contains.")
{
	Set<int32> mySet = { 1, 3 };

	CHECK(mySet.Contains(1));
	CHECK(!mySet.Contains(2));
}

CLASS_TEST_CASE("Add copy value.")
{
	Set<int32> mySet;
	mySet.Add(1);

	CHECK(mySet.GetCount() == 1);
	CHECK(mySet.Contains(1));
}

CLASS_TEST_CASE("Add move value.")
{
	MyStruct myStruct = { 1 };
	Set<MyStruct> mySet;
	mySet.Add(std::move(myStruct));

	CHECK(mySet.GetCount() == 1);
	CHECK(mySet.Contains({ 1 }));
}

CLASS_TEST_CASE("Add copy range.")
{
	Set<int32> mySetA;
	Set<int32> mySetB = { 3, 4, 5 };
	mySetA.Add(mySetB);

	CHECK(mySetA.GetCount() == 3);
	CHECK(mySetA.Contains(3));
	CHECK(mySetA.Contains(4));
	CHECK(mySetA.Contains(5));
}

CLASS_TEST_CASE("Add move range.")
{
	Set<int32> mySetA;
	Set<int32> mySetB = { 3, 4, 5 };
	mySetA.Add(std::move(mySetB));

	CHECK(mySetA.GetCount() == 3);
	CHECK(mySetA.Contains(3));
	CHECK(mySetA.Contains(4));
	CHECK(mySetA.Contains(5));
}

CLASS_TEST_CASE("Remove value.")
{
	Set<int32> mySet = { 1, 2, 3 };
	mySet.Remove(2);

	CHECK(mySet.GetCount() == 2);
	CHECK(mySet.Contains(1));
	CHECK(!mySet.Contains(2));
	CHECK(mySet.Contains(3));
}

CLASS_TEST_CASE("Remove range.")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB = { 1, 4 };
	mySetA.Remove(mySetB);

	CHECK(mySetA.GetCount() == 2);
	CHECK(!mySetA.Contains(1));
	CHECK(mySetA.Contains(2));
	CHECK(mySetA.Contains(3));
}

CLASS_TEST_CASE("RemoveAll.")
{
	Set<int32> mySet = { 1, 2, 3 };
	mySet.RemoveAll();

	CHECK(mySet.GetCount() == 0);
	CHECK(!mySet.Contains(1));
	CHECK(!mySet.Contains(2));
	CHECK(!mySet.Contains(3));
}

CLASS_TEST_CASE("Range-based for loop pre-increment.")
{

	Set<int32> mySet = { 1, 2, 3 };
	auto iterator = mySet.begin();
	CHECK(*iterator == 1);
	CHECK(*++iterator == 2);
	CHECK(*++iterator == 3);
	CHECK(++iterator == mySet.end());
}

CLASS_TEST_CASE("Range-based for loop post-increment.")
{

	Set<int32> mySet = { 1, 2, 3 };
	auto iterator = mySet.begin();
	CHECK(*iterator++ == 1);
	CHECK(*iterator++ == 2);
	CHECK(*iterator++ == 3);
	CHECK(iterator == mySet.end());
}

CLASS_TEST_CASE("Range-based for loop.")
{
	Set<int32> mySet = { 1, 2, 3 };
	int32 i = -1;
	int32 checks[] = { 1, 2, 3 };
	for (int32 value : mySet)
	{
		i++;
		CHECK(value == checks[i]);
	}
}
