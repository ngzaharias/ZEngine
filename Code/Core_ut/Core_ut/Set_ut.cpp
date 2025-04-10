#include "CorePCH.h"

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

TEST_CASE("Set. Default constructor.")
{
	Set<int32> mySet;

	CHECK(mySet.GetCount() == 0);
}

TEST_CASE("Set. Copy constructor.")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB = mySetA;

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

TEST_CASE("Set. Move constructor.")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB = std::move(mySetA);

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

TEST_CASE("Set. initializer_list constructor.")
{
	Set<int32> mySet = { 1, 2, 3 };

	CHECK(mySet.GetCount() == 3);
	CHECK(mySet.Contains(1));
	CHECK(mySet.Contains(2));
	CHECK(mySet.Contains(3));
}

TEST_CASE("Set. operator=(Set&&).")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB;
	mySetB = std::move(mySetA);

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

TEST_CASE("Set. operator=(const Set&).")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB;
	mySetB = mySetA;

	CHECK(mySetB.GetCount() == 3);
	CHECK(mySetB.Contains(1));
	CHECK(mySetB.Contains(2));
	CHECK(mySetB.Contains(3));
}

TEST_CASE("Set. operator=(initializer_list&&).")
{
	Set<int32> mySet;
	mySet = { 1, 2, 3 };

	CHECK(mySet.GetCount() == 3);
	CHECK(mySet.Contains(1));
	CHECK(mySet.Contains(2));
	CHECK(mySet.Contains(3));
}

TEST_CASE("Set. IsEmpty.")
{
	Set<int32> mySet;
	CHECK(mySet.IsEmpty());

	mySet = { 1, 2, 3 };
	CHECK(!mySet.IsEmpty());
}

TEST_CASE("Set. GetCount.")
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

TEST_CASE("Set. Contains.")
{
	Set<int32> mySet = { 1, 3 };

	CHECK(mySet.Contains(1));
	CHECK(!mySet.Contains(2));
}

TEST_CASE("Set. Add copy value.")
{
	Set<int32> mySet;
	mySet.Add(1);

	CHECK(mySet.GetCount() == 1);
	CHECK(mySet.Contains(1));
}

TEST_CASE("Set. Add move value.")
{
	MyStruct myStruct = { 1 };
	Set<MyStruct> mySet;
	mySet.Add(std::move(myStruct));

	CHECK(mySet.GetCount() == 1);
	CHECK(mySet.Contains({ 1 }));
}

TEST_CASE("Set. Add copy range.")
{
	Set<int32> mySetA;
	Set<int32> mySetB = { 3, 4, 5 };
	mySetA.Add(mySetB);

	CHECK(mySetA.GetCount() == 3);
	CHECK(mySetA.Contains(3));
	CHECK(mySetA.Contains(4));
	CHECK(mySetA.Contains(5));
}

TEST_CASE("Set. Add move range.")
{
	Set<int32> mySetA;
	Set<int32> mySetB = { 3, 4, 5 };
	mySetA.Add(std::move(mySetB));

	CHECK(mySetA.GetCount() == 3);
	CHECK(mySetA.Contains(3));
	CHECK(mySetA.Contains(4));
	CHECK(mySetA.Contains(5));
}

TEST_CASE("Set. Remove value.")
{
	Set<int32> mySet = { 1, 2, 3 };
	mySet.Remove(2);

	CHECK(mySet.GetCount() == 2);
	CHECK(mySet.Contains(1));
	CHECK(!mySet.Contains(2));
	CHECK(mySet.Contains(3));
}

TEST_CASE("Set. Remove range.")
{
	Set<int32> mySetA = { 1, 2, 3 };
	Set<int32> mySetB = { 1, 4 };
	mySetA.Remove(mySetB);

	CHECK(mySetA.GetCount() == 2);
	CHECK(!mySetA.Contains(1));
	CHECK(mySetA.Contains(2));
	CHECK(mySetA.Contains(3));
}

TEST_CASE("Set. RemoveAll.")
{
	Set<int32> mySet = { 1, 2, 3 };
	mySet.RemoveAll();

	CHECK(mySet.GetCount() == 0);
	CHECK(!mySet.Contains(1));
	CHECK(!mySet.Contains(2));
	CHECK(!mySet.Contains(3));
}

TEST_CASE("Set. Range-based for loop pre-increment.")
{

	Set<int32> mySet = { 1, 2, 3 };
	auto iterator = mySet.begin();
	CHECK(*iterator == 1);
	CHECK(*++iterator == 2);
	CHECK(*++iterator == 3);
	CHECK(++iterator == mySet.end());
}

TEST_CASE("Set. Range-based for loop post-increment.")
{

	Set<int32> mySet = { 1, 2, 3 };
	auto iterator = mySet.begin();
	CHECK(*iterator++ == 1);
	CHECK(*iterator++ == 2);
	CHECK(*iterator++ == 3);
	CHECK(iterator == mySet.end());
}

TEST_CASE("Set. Range-based for loop.")
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
