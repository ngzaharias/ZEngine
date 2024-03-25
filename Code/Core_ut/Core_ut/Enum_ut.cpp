#include <Catch2/catch.hpp>

#include <Core/EnumHelpers.h>

namespace
{
	enum class EnumA
	{
		First = 1,
		Second,
		Third,
		Fifty = 50,
		Leet = 128,
	};
}

TEST_CASE("enum::EnumToCount")
{
	CHECK(EnumToCount<EnumA>() == 5);
}

TEST_CASE("enum::EnumToString")
{
	CHECK(EnumToString<EnumA>() == "EnumA");
	CHECK(EnumToString(EnumA::First) == "First");
	CHECK(EnumToString(EnumA::Second) == "Second");
	CHECK(EnumToString(EnumA::Third) == "Third");
	CHECK(EnumToString(EnumA::Fifty) == "Fifty");
	CHECK(EnumToString(EnumA::Leet) == "Leet");
}

TEST_CASE("enum::EnumToValue")
{
	CHECK(EnumToValue(EnumA::First) == 1);
	CHECK(EnumToValue(EnumA::Second) == 2);
	CHECK(EnumToValue(EnumA::Third) == 3);
	CHECK(EnumToValue(EnumA::Fifty) == 50);
	CHECK(EnumToValue(EnumA::Leet) == 128);
}

TEST_CASE("enum::IndexToEnum")
{
	CHECK(IndexToEnum<EnumA>(0) == EnumA::First);
	CHECK(IndexToEnum<EnumA>(1) == EnumA::Second);
	CHECK(IndexToEnum<EnumA>(2) == EnumA::Third);
	CHECK(IndexToEnum<EnumA>(3) == EnumA::Fifty);
	CHECK(IndexToEnum<EnumA>(4) == EnumA::Leet);
}

TEST_CASE("enum::StringToEnum")
{
	CHECK(StringToEnum<EnumA>("First") == EnumA::First);
	CHECK(StringToEnum<EnumA>("Second") == EnumA::Second);
	CHECK(StringToEnum<EnumA>("Third") == EnumA::Third);
	CHECK(StringToEnum<EnumA>("Fifty") == EnumA::Fifty);
	CHECK(StringToEnum<EnumA>("Leet") == EnumA::Leet);
}

TEST_CASE("enum::ValueToEnum")
{
	CHECK(ValueToEnum<EnumA>(1) == EnumA::First);
	CHECK(ValueToEnum<EnumA>(2) == EnumA::Second);
	CHECK(ValueToEnum<EnumA>(3) == EnumA::Third);
	CHECK(ValueToEnum<EnumA>(50) == EnumA::Fifty);
	CHECK(ValueToEnum<EnumA>(128) == EnumA::Leet);
}