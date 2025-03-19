#include <Catch2/catch.hpp>

#include "Core/Flags.h"

#define BIT32(value) (1<<value)

namespace
{
	enum MyEnum
	{
		A = BIT32(0),
		B = BIT32(1),
		C = BIT32(2),
		D = BIT32(3),
		E = BIT32(4),
	};
	using MyFlags = Flags<MyEnum>;
}

TEST_CASE("Flags. Has(Enum).")
{
	const MyFlags flags = MyFlags(MyEnum::A);
	CHECK(flags.Has(MyEnum::A));
	CHECK(!flags.Has(MyEnum::B));
}

TEST_CASE("Flags. HasAll(Flags).")
{
	MyFlags flagsA; flagsA.Raise(MyEnum::A); flagsA.Raise(MyEnum::B); flagsA.Raise(MyEnum::D);
	MyFlags flagsB; flagsB.Raise(MyEnum::A); flagsB.Raise(MyEnum::B);
	MyFlags flagsC; flagsC.Raise(MyEnum::A); flagsC.Raise(MyEnum::C);
	CHECK(flagsA.HasAll(flagsB));
	CHECK(!flagsA.HasAll(flagsC));
}

TEST_CASE("Flags. HasAny().")
{
	const MyFlags flagsA = MyFlags(MyEnum::A);
	const MyFlags flagsB = {};
	CHECK(flagsA.HasAny());
	CHECK(!flagsB.HasAny());
}

TEST_CASE("Flags. HasAny(Flags).")
{
	MyFlags flagsA; flagsA.Raise(MyEnum::A); flagsA.Raise(MyEnum::D);
	MyFlags flagsB; flagsB.Raise(MyEnum::A); flagsB.Raise(MyEnum::B);
	MyFlags flagsC; flagsC.Raise(MyEnum::C); flagsC.Raise(MyEnum::E);
	CHECK(flagsA.HasAny(flagsB));
	CHECK(!flagsA.HasAny(flagsC));
}

TEST_CASE("Flags. HasOnly(Enum).")
{
	const MyFlags flags = MyFlags(MyEnum::A);
	CHECK(flags.HasOnly(MyEnum::A));
	CHECK(!flags.HasOnly(MyEnum::B));
}

TEST_CASE("Flags. HasOnly(Flags).")
{
	MyFlags flagsA; flagsA.Raise(MyEnum::A); flagsA.Raise(MyEnum::B);
	MyFlags flagsB; flagsB.Raise(MyEnum::A); flagsB.Raise(MyEnum::B);
	MyFlags flagsC; flagsC.Raise(MyEnum::A); flagsC.Raise(MyEnum::C);
	CHECK(flagsA.HasOnly(flagsB));
	CHECK(!flagsA.HasOnly(flagsC));
}

TEST_CASE("Flags. HasNone().")
{
	const MyFlags flagsA = {};
	const MyFlags flagsB = MyFlags(MyEnum::A);
	CHECK(flagsA.HasNone());
	CHECK(!flagsB.HasNone());
}

TEST_CASE("Flags. Clear(Enum).")
{
	MyFlags flags; flags.Raise(MyEnum::A); flags.Raise(MyEnum::B);
	REQUIRE(flags.Has(MyEnum::A));
	REQUIRE(flags.Has(MyEnum::B));
	flags.Clear(MyEnum::A);
	CHECK(!flags.Has(MyEnum::A));
	CHECK(flags.Has(MyEnum::B));
}

TEST_CASE("Flags. Clear(Flags).")
{
	MyFlags flagsA; flagsA.Raise(MyEnum::A); flagsA.Raise(MyEnum::B);
	MyFlags flagsB; flagsB.Raise(MyEnum::A); flagsB.Raise(MyEnum::C);
	REQUIRE(flagsA.Has(MyEnum::A));
	REQUIRE(flagsA.Has(MyEnum::B));
	flagsA.Clear(flagsB);
	CHECK(!flagsA.Has(MyEnum::A));
	CHECK(flagsA.Has(MyEnum::B));
}

TEST_CASE("Flags. ClearAll().")
{
	MyFlags flags; flags.Raise(MyEnum::A); flags.Raise(MyEnum::B);
	REQUIRE(flags.Has(MyEnum::A));
	REQUIRE(flags.Has(MyEnum::B));
	flags.ClearAll();
	CHECK(!flags.Has(MyEnum::A));
	CHECK(!flags.Has(MyEnum::B));
}

TEST_CASE("Flags. Raise(Enum).")
{
	MyFlags flags = MyFlags(MyEnum::B);
	REQUIRE(!flags.Has(MyEnum::A));
	REQUIRE(flags.Has(MyEnum::B));
	flags.Raise(MyEnum::A);
	CHECK(flags.Has(MyEnum::A));
	CHECK(flags.Has(MyEnum::B));
}

TEST_CASE("Flags. Raise(Flags).")
{
	MyFlags flagsA; flagsA.Raise(MyEnum::B); flagsA.Raise(MyEnum::C);
	MyFlags flagsB; flagsB.Raise(MyEnum::A); flagsB.Raise(MyEnum::B);
	REQUIRE(!flagsA.Has(MyEnum::A));
	REQUIRE(flagsA.Has(MyEnum::B));
	flagsA.Raise(flagsB);
	CHECK(flagsA.Has(MyEnum::A));
	CHECK(flagsA.Has(MyEnum::B));
}

TEST_CASE("Flags. RaiseAll().")
{
	MyFlags flags = MyFlags(MyEnum::A);
	REQUIRE(flags.Has(MyEnum::A));
	REQUIRE(!flags.Has(MyEnum::B));
	REQUIRE(!flags.Has(MyEnum::C));
	REQUIRE(!flags.Has(MyEnum::D));
	REQUIRE(!flags.Has(MyEnum::E));
	flags.RaiseAll();
	CHECK(flags.Has(MyEnum::A));
	CHECK(flags.Has(MyEnum::B));
	CHECK(flags.Has(MyEnum::C));
	CHECK(flags.Has(MyEnum::D));
	CHECK(flags.Has(MyEnum::E));
}

TEST_CASE("Flags. Set(Enum, bool).")
{
	MyFlags flags = MyFlags(MyEnum::A);
	REQUIRE(flags.Has(MyEnum::A));
	REQUIRE(!flags.Has(MyEnum::B));
	flags.Set(MyEnum::A, false);
	flags.Set(MyEnum::B, true);
	CHECK(!flags.Has(MyEnum::A));
	CHECK(flags.Has(MyEnum::B));
}

TEST_CASE("Flags. Toggle(Enum).")
{
	MyFlags flags = MyFlags(MyEnum::A);
	REQUIRE(flags.Has(MyEnum::A));
	REQUIRE(!flags.Has(MyEnum::B));
	flags.Toggle(MyEnum::A);
	flags.Toggle(MyEnum::B);
	CHECK(!flags.Has(MyEnum::A));
	CHECK(flags.Has(MyEnum::B));
}

#undef BIT32