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

TEST_CASE("Delegate. A connection generates a handle.")
{
	Delegate<void()> delegate;
	CHECK(delegate.Connect(MyFunctionA) == 0);
}

TEST_CASE("Delegate. Multiple connections generate unique handles.")
{
	Delegate<void()> delegate;
	CHECK(delegate.Connect(MyFunctionA) == 0);
	CHECK(delegate.Connect(MyFunctionA) == 1);
	CHECK(delegate.Connect(MyFunctionA) == 2);
	CHECK(delegate.Connect(MyFunctionA) == 3);
	CHECK(delegate.Connect(MyFunctionA) == 4);
}

TEST_CASE("Delegate. A connection can be disconnected.")
{
	s_Variables.RemoveAll();
	int32 handle = -1;

	Delegate<void()> delegate;
	handle = delegate.Connect(MyFunctionA);
	delegate.Publish();
	delegate.Disconnect(handle);
	delegate.Publish();
	REQUIRE(s_Variables.GetCount() == 1);
	CHECK(s_Variables[0] == 1);
	CHECK(handle == 0);
}

TEST_CASE("Delegate. A disconnection doesn't alter the call order of the other connections.")
{
	s_Variables.RemoveAll();
	int32 handleA = -1;
	int32 handleB = -1;
	int32 handleC = -1;

	Delegate<void()> delegate;
	handleA = delegate.Connect(MyFunctionC1);
	handleB = delegate.Connect(MyFunctionC2);
	handleC = delegate.Connect(MyFunctionC3);
	delegate.Publish();
	
	CHECK(delegate.Disconnect(handleB));

	delegate.Publish();

	REQUIRE(s_Variables.GetCount() == 5);
	CHECK(s_Variables[0] == 1);
	CHECK(s_Variables[1] == 2);
	CHECK(s_Variables[2] == 3);
	CHECK(s_Variables[3] == 1);
	CHECK(s_Variables[4] == 3);
}