#include "CorePCH.h"

#include "Core/Algorithms.h"
#include "Core/SparseSet.h"
#include "Core/Types.h"

#define CLASS_TEST_CASE(name) TEST_CASE("SparseSet. " name, "[SparseSet]")

namespace
{
	struct MyStruct
	{
	public:
		bool operator==(const MyStruct& rhs) const { return m_Index == rhs.m_Index; }
		int32 GetIndex() const { return m_Index; };
		int32 m_Index = -1;
	};
}

CLASS_TEST_CASE("Contains.")
{
	SparseSet<int32, float> myArray;
	CHECK(!myArray.Contains(0));
	CHECK(!myArray.Contains(1));

	myArray.Set(0, 1.337f);
	CHECK(myArray.Contains(0));
	CHECK(!myArray.Contains(1));
}

CLASS_TEST_CASE("GetCapacity.")
{
	SparseSet<int32, float> myArray;
	CHECK(myArray.GetCapacity() == 0);

	myArray.Set(0, 1.f);
	CHECK(myArray.GetCapacity() == 1);

	myArray.Set(3, 2.f);
	myArray.Set(999, 3.f);
	myArray.Set(123, 4.f);
	CHECK(myArray.GetCapacity() == 4);

	myArray.Set(666, 5.f);
	CHECK(myArray.GetCapacity() == 6);
}

CLASS_TEST_CASE("GetCount.")
{
	SparseSet<int32, float> myArray;
	CHECK(myArray.GetCount() == 0);

	myArray.Set(0, 1.337f);
	CHECK(myArray.GetCount() == 1);

	myArray.Set(1, 1.337f);
	myArray.Set(2, 1.337f);
	myArray.Set(3, 1.337f);
	CHECK(myArray.GetCount() == 4);

	myArray.Set(3, 0.666f);
	CHECK(myArray.GetCount() == 4);
}

CLASS_TEST_CASE("GetIndex for integral types returns its value.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 0.f);
	myArray.Set(1, 0.f);
	myArray.Set(9, 0.f);

	CHECK(myArray.GetIndex(0) == 0);
	CHECK(myArray.GetIndex(1) == 1);
	CHECK(myArray.GetIndex(9) == 9);
}

CLASS_TEST_CASE("GetIndex for custom types uses its GetIndex method.")
{
	SparseSet<MyStruct, float> myArray;
	myArray.Set(MyStruct{ 0 }, 0.f);
	myArray.Set(MyStruct{ 1 }, 0.f);
	myArray.Set(MyStruct{ 9 }, 0.f);

	CHECK(myArray.GetIndex(MyStruct{ 0 }) == 0);
	CHECK(myArray.GetIndex(MyStruct{ 1 }) == 1);
	CHECK(myArray.GetIndex(MyStruct{ 9 }) == 9);
}

CLASS_TEST_CASE("Get.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.337f);

	CHECK(myArray.Get(0) == 1.337f);
}

CLASS_TEST_CASE("Emplace.")
{
	SparseSet<int32, float> myArray;
	myArray.Emplace(0, 1.f);
	myArray.Emplace(3, 2.f);
	myArray.Emplace(10, 3.f);
	myArray.Emplace(999, 4.f);

	CHECK(myArray.GetCount() == 4);
	CHECK(myArray.Get(0) == 1.f);
	CHECK(myArray.Get(3) == 2.f);
	CHECK(myArray.Get(10) == 3.f);
	CHECK(myArray.Get(999) == 4.f);

	//INFO("Negative indexes aren't supported.");
	//CHECK_THROWS(myArray.Emplace(-12, 5.f));
	//CHECK_THROWS(myArray.Emplace(INT32_MAX, 4.f));
}

CLASS_TEST_CASE("Set.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	CHECK(myArray.GetCount() == 4);
	CHECK(myArray.Get(0) == 1.f);
	CHECK(myArray.Get(3) == 2.f);
	CHECK(myArray.Get(10) == 3.f);
	CHECK(myArray.Get(999) == 4.f);

	myArray.Set(999, 5.f);
	CHECK(myArray.Get(999) == 5.f);

	//INFO("Negative indexes aren't supported.");
	//CHECK_THROWS(myArray.Set(-12, 5.f));
	//CHECK_THROWS(myArray.Set(INT32_MAX, 4.f));
}

CLASS_TEST_CASE("Set using move.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, std::move(1.f));
	myArray.Set(3, std::move(2.f));
	myArray.Set(10, std::move(3.f));
	myArray.Set(999, std::move(4.f));

	CHECK(myArray.GetCount() == 4);
	CHECK(myArray.Get(0) == 1.f);
	CHECK(myArray.Get(3) == 2.f);
	CHECK(myArray.Get(10) == 3.f);
	CHECK(myArray.Get(999) == 4.f);

	myArray.Set(999, std::move(5.f));
	CHECK(myArray.Get(999) == 5.f);

	//INFO("Negative indexes aren't supported.");
	//CHECK_THROWS(myArray.Set(-12, 5.f));
	//CHECK_THROWS(myArray.Set(INT32_MAX, 4.f));
}

CLASS_TEST_CASE("Removing from the first should do a swap with the last item in the list.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	myArray.Remove(0);

	CHECK(!myArray.Contains(0));
	CHECK(myArray.Contains(3));
	CHECK(myArray.Contains(10));
	CHECK(myArray.Contains(999));
}

CLASS_TEST_CASE("Removing from the middle should do a swap with the last item in the list.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	myArray.Remove(3);

	REQUIRE(myArray.Contains(0));
	REQUIRE(!myArray.Contains(3));
	REQUIRE(myArray.Contains(10));
	REQUIRE(myArray.Contains(999));
}

CLASS_TEST_CASE("Removing from the end should just pop the last item in the array.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	myArray.Remove(999);

	REQUIRE(myArray.Contains(0));
	REQUIRE(myArray.Contains(3));
	REQUIRE(myArray.Contains(10));
	REQUIRE(!myArray.Contains(999));
}

CLASS_TEST_CASE("RemoveAll.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	CHECK(myArray.GetCount() == 4);
	myArray.RemoveAll();
	CHECK(myArray.GetCount() == 0);
}

CLASS_TEST_CASE("Reserve.")
{
	SparseSet<int32, float> myArray;
	myArray.Reserve(10);

	CHECK(myArray.GetCapacity() == 10);
}

CLASS_TEST_CASE("Key: Custom.")
{
	SparseSet<MyStruct, float> myArray;
	myArray.Emplace(MyStruct{ 0 }, 1.f);
	myArray.Emplace(MyStruct{ 3 }, 2.f);
	myArray.Set(MyStruct{ 10 }, 3.f);
	myArray.Set(MyStruct{ 999 }, 4.f);

	CHECK(myArray.GetCount() == 4);

	REQUIRE(myArray.Contains({ 0 }));
	REQUIRE(myArray.Contains({ 3 }));
	REQUIRE(myArray.Contains({ 10 }));
	REQUIRE(myArray.Contains({ 999 }));

	CHECK(!myArray.Contains({ 123 }));
	CHECK(!myArray.Contains({ 456 }));
	CHECK(!myArray.Contains({ 789 }));

	CHECK(myArray.Get({ 0 }) == 1.f);
	CHECK(myArray.Get({ 3 }) == 2.f);
	CHECK(myArray.Get({ 10 }) == 3.f);
	CHECK(myArray.Get({ 999 }) == 4.f);

	myArray.Remove({ 0 });
	myArray.Remove({ 10 });

	CHECK(myArray.GetCount() == 2);
	CHECK(myArray.Contains({ 3 }));
	CHECK(myArray.Contains({ 999 }));
	CHECK(!myArray.Contains({ 0 }));
	CHECK(!myArray.Contains({ 10 }));

	myArray.RemoveAll();
	CHECK(myArray.GetCount() == 0);
}

CLASS_TEST_CASE("Key: Int32.")
{
	SparseSet<int32, float> myArray;
	myArray.Emplace(0, 1.f);
	myArray.Emplace(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	CHECK(myArray.GetCount() == 4);

	REQUIRE(myArray.Contains(0));
	REQUIRE(myArray.Contains(3));
	REQUIRE(myArray.Contains(10));
	REQUIRE(myArray.Contains(999));

	CHECK(!myArray.Contains(123));
	CHECK(!myArray.Contains(456));
	CHECK(!myArray.Contains(789));

	CHECK(myArray.Get(0) == 1.f);
	CHECK(myArray.Get(3) == 2.f);
	CHECK(myArray.Get(10) == 3.f);
	CHECK(myArray.Get(999) == 4.f);

	myArray.Remove(0);
	myArray.Remove(10);

	CHECK(myArray.GetCount() == 2);
	CHECK(myArray.Contains(3));
	CHECK(myArray.Contains(999));
	CHECK(!myArray.Contains(0));
	CHECK(!myArray.Contains(10));

	myArray.RemoveAll();
	CHECK(myArray.GetCount() == 0);
}

CLASS_TEST_CASE("Iterator.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	{
		INFO("Pre-increment: Key.");

		auto iterator = myArray.begin();
		CHECK((*iterator).m_Key == 0);
		CHECK((*++iterator).m_Key == 3);
		CHECK((*++iterator).m_Key == 10);
		CHECK((*++iterator).m_Key == 999);
		CHECK(++iterator == myArray.end());
	}
	{
		INFO("Pre-increment: Value.");

		auto iterator = myArray.begin();
		CHECK((*iterator).m_Value == 1.f);
		CHECK((*++iterator).m_Value == 2.f);
		CHECK((*++iterator).m_Value == 3.f);
		CHECK((*++iterator).m_Value == 4.f);
		CHECK(++iterator == myArray.end());
	}

	{
		INFO("Post-increment: Key.");

		auto iterator = myArray.begin();
		CHECK((*iterator++).m_Key == 0);
		CHECK((*iterator++).m_Key == 3);
		CHECK((*iterator++).m_Key == 10);
		CHECK((*iterator++).m_Key == 999);
		CHECK(iterator == myArray.end());
	}

	{
		INFO("Post-increment: Value.");

		auto iterator = myArray.begin();
		CHECK((*iterator++).m_Value == 1.f);
		CHECK((*iterator++).m_Value == 2.f);
		CHECK((*iterator++).m_Value == 3.f);
		CHECK((*iterator++).m_Value == 4.f);
		CHECK(iterator == myArray.end());
	}
}

CLASS_TEST_CASE("Range-based for loop.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	int32 i = -1;
	std::pair<int32, float> checks[] = { { 0, 1.f },{ 3, 2.f },{ 10, 3.f },{ 999, 4.f } };
	for (auto&& iterator : myArray)
	{
		i++;
		CHECK(iterator.m_Key == checks[i].first);
		CHECK(iterator.m_Value == checks[i].second);

		iterator.m_Value = 1337.f;
	}

	// check that the values were modified
	for (auto&& iterator : myArray)
		CHECK(iterator.m_Value == 1337.f);
}

CLASS_TEST_CASE("Range-based for loop with structured binding.")
{
	SparseSet<int32, float> myArray;
	myArray.Set(0, 1.f);
	myArray.Set(3, 2.f);
	myArray.Set(10, 3.f);
	myArray.Set(999, 4.f);

	int32 i = -1;
	std::pair<int32, float> checks[] = { { 0, 1.f },{ 3, 2.f },{ 10, 3.f },{ 999, 4.f } };
	for (auto&& [key, value] : myArray)
	{
		i++;
		CHECK(key == checks[i].first);
		CHECK(value == checks[i].second);
		value = 1337.f;
	}

	// check that the values were modified and the keys weren't
	i = -1;
	for (auto&& [key, value] : myArray)
	{
		i++;
		CHECK(key == checks[i].first);
		CHECK(value == 1337.f);
	}
}