#include <Catch2/catch.hpp>

#include "Core/Delegate.h"

#include <iostream>

namespace
{
	static int32 s_Variable = 0;
	void MyFunctionA() { s_Variable++; }
	void MyFunctionB(int32 val) { s_Variable = val; }

	struct MyStruct
	{
		void MyFunctionA() { m_Variable++; }
		void MyFunctionB() const { m_Variable++; }
		mutable int32 m_Variable = 0;
	};
}

TEST_CASE("Delegate. Connect a function with no arguments.")
{
	s_Variable = 0;

	Delegate<void()> delegate;
	delegate.Connect(MyFunctionA);
	delegate.Publish();
	CHECK(s_Variable == 1);
}

TEST_CASE("Delegate. Connect a function with one argument.")
{
	s_Variable = 0;

	Delegate<void(int32)> delegate;
	delegate.Connect(MyFunctionB);
	delegate.Publish(3);
	CHECK(s_Variable == 3);
}

TEST_CASE("Delegate. Connect a object and method.")
{
	MyStruct objectA;
	const MyStruct objectB;

	Delegate<void()> delegate;
	delegate.Connect(objectA, &MyStruct::MyFunctionA);
	delegate.Connect(objectB, &MyStruct::MyFunctionB);
	delegate.Publish();
	CHECK(objectA.m_Variable == 1);
	CHECK(objectB.m_Variable == 1);
}

TEST_CASE("Delegate. Connect multiple functions to the same delegate.")
{
	s_Variable = 0;

	Delegate<void()> delegate;
	delegate.Connect(MyFunctionA);
	delegate.Connect(MyFunctionA);
	delegate.Connect(MyFunctionA);
	delegate.Connect(MyFunctionA);
	delegate.Publish();
	CHECK(s_Variable == 4);
}