#include <Catch2/catch.hpp>

#include "Core/Delegate.h"

#include <iostream>

namespace
{
	static Array<int32> s_Variables = 0;
	void MyFunctionA() { s_Variables.Append(1); }
	void MyFunctionB(int32 val) { s_Variables.Append(val); }
	void MyFunctionC1() { s_Variables.Append(1); }
	void MyFunctionC2() { s_Variables.Append(2); }
	void MyFunctionC3() { s_Variables.Append(3); }

	struct MyStruct
	{
		void MyFunctionA() { m_Variables.Append(1); }
		void MyFunctionB() const { m_Variables.Append(1); }
		mutable Array<int32> m_Variables = {};
	};
}

TEST_CASE("Delegate. Connect a function with no arguments.")
{
	s_Variables.RemoveAll();

	Delegate<void()> delegate;
	delegate.Connect(MyFunctionA);
	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 1);
	CHECK(s_Variables[0] == 1);
}

TEST_CASE("Delegate. Connect a function with one argument.")
{
	s_Variables.RemoveAll();

	Delegate<void(int32)> delegate;
	delegate.Connect(MyFunctionB);
	delegate.Publish(3);

	REQUIRE(s_Variables.GetCount() == 1);
	CHECK(s_Variables[0] == 3);
}

TEST_CASE("Delegate. Connect a object and method.")
{
	MyStruct objectA;
	const MyStruct objectB;

	Delegate<void()> delegate;
	delegate.Connect(objectA, &MyStruct::MyFunctionA);
	delegate.Connect(objectB, &MyStruct::MyFunctionB);
	delegate.Publish();

	REQUIRE(objectA.m_Variables.GetCount() == 1);
	REQUIRE(objectB.m_Variables.GetCount() == 1);
	CHECK(objectA.m_Variables[0] == 1);
	CHECK(objectB.m_Variables[0] == 1);
}

TEST_CASE("Delegate. Connect the same function multiple times to the same delegate.")
{
	s_Variables.RemoveAll();

	Delegate<void()> delegate;
	delegate.Connect(MyFunctionA);
	delegate.Connect(MyFunctionA);
	delegate.Connect(MyFunctionA);
	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 3);
	CHECK(s_Variables[0] == 1);
	CHECK(s_Variables[1] == 1);
	CHECK(s_Variables[2] == 1);
}

TEST_CASE("Delegate. Connect multiple functions to the same delegate.")
{
	s_Variables.RemoveAll();

	Delegate<void()> delegate;
	delegate.Connect(MyFunctionC1);
	delegate.Connect(MyFunctionC2);
	delegate.Connect(MyFunctionC3);
	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 3);
	CHECK(s_Variables[0] == 1);
	CHECK(s_Variables[1] == 2);
	CHECK(s_Variables[2] == 3);
}

TEST_CASE("Delegate. A connection can be disconnected.")
{
	s_Variables.RemoveAll();

	Delegate<void()> delegate;
	auto handle = delegate.Connect(MyFunctionA);

	delegate.Publish();
	handle.Disconnect();
	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 1);
	CHECK(s_Variables[0] == 1);
}

TEST_CASE("Delegate. A disconnection doesn't alter the call order of the other connections.")
{
	s_Variables.RemoveAll();

	Delegate<void()> delegate;
	auto handleA = delegate.Connect(MyFunctionC1);
	auto handleB = delegate.Connect(MyFunctionC2);
	auto handleC = delegate.Connect(MyFunctionC3);

	delegate.Publish();
	handleB.Disconnect();
	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 5);
	CHECK(s_Variables[0] == 1);
	CHECK(s_Variables[1] == 2);
	CHECK(s_Variables[2] == 3);
	CHECK(s_Variables[3] == 1);
	CHECK(s_Variables[4] == 3);
}

TEST_CASE("Delegate. DelegateHandles can capture multiple.")
{
	s_Variables.RemoveAll();

	Delegate<void()> delegate;
	DelegateCollection collection =
	{
		delegate.Connect(MyFunctionC1),
		delegate.Connect(MyFunctionC2),
		delegate.Connect(MyFunctionC3),
	};

	delegate.Publish();
	collection.Disconnect();
	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 3);
	CHECK(s_Variables[0] == 3);
	CHECK(s_Variables[1] == 2);
	CHECK(s_Variables[2] == 1);
}