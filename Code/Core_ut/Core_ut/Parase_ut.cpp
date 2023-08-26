#include <Catch2/catch.hpp>

#include <Core/Parse.h>
#include <Core/String.h>
#include <Core/StringView.h>

TEST_CASE("str::Parse")
{
	SECTION("double")
	{
		CHECK(str::Parse<double>("1") == 1.);
		CHECK(str::Parse<double>(".1") == 0.1);
		CHECK(str::Parse<double>("1.1") == 1.1);

		CHECK(str::Parse<double>("+2") == 2.);
		CHECK(str::Parse<double>("+.2") == 0.2);
		CHECK(str::Parse<double>("+2.2") == 2.2);

		CHECK(str::Parse<double>("-3") == -3.);
		CHECK(str::Parse<double>("-.3") == -0.3);
		CHECK(str::Parse<double>("-3.3") == -3.3);
	}

	SECTION("float")
	{
		CHECK(str::Parse<float>("1") == 1.f);
		CHECK(str::Parse<float>(".1") == 0.1f);
		CHECK(str::Parse<float>("1.1") == 1.1f);
		CHECK(str::Parse<float>("1.f") == 1.f);
		CHECK(str::Parse<float>(".1f") == 0.1f);
		CHECK(str::Parse<float>("1.1f") == 1.1f);
		CHECK(str::Parse<float>("1.11f") == 1.11f);

		CHECK(str::Parse<float>("+2") == 2.f);
		CHECK(str::Parse<float>("+.2") == 0.2f);
		CHECK(str::Parse<float>("+2.2") == 2.2f);
		CHECK(str::Parse<float>("+.2f") == 0.2f);
		CHECK(str::Parse<float>("+2.2f") == 2.2f);
		CHECK(str::Parse<float>("+2.22f") == 2.22f);

		CHECK(str::Parse<float>("-3") == -3.f);
		CHECK(str::Parse<float>("-.3") == -0.3f);
		CHECK(str::Parse<float>("-3.3") == -3.3f);
		CHECK(str::Parse<float>("-.3f") == -0.3f);
		CHECK(str::Parse<float>("-3.3f") == -3.3f);
		CHECK(str::Parse<float>("-3.33f") == -3.33f);
	}

	SECTION("int8")
	{
		CHECK(str::Parse<int8>("0") == 0);
		CHECK(str::Parse<int8>("1") == 1);

		CHECK(str::Parse<int8>("+1") == 1);
		CHECK(str::Parse<int8>("-1") == -1);
	}

	SECTION("int16")
	{
		CHECK(str::Parse<int16>("0") == 0);
		CHECK(str::Parse<int16>("1") == 1);

		CHECK(str::Parse<int16>("+1") == 1);
		CHECK(str::Parse<int16>("-1") == -1);
	}

	SECTION("int32")
	{
		CHECK(str::Parse<int32>("0") == 0);
		CHECK(str::Parse<int32>("1") == 1);

		CHECK(str::Parse<int32>("+1") == 1);
		CHECK(str::Parse<int32>("-1") == -1);
	}

	SECTION("int64")
	{
		CHECK(str::Parse<int64>("0") == 0);
		CHECK(str::Parse<int64>("1") == 1);

		CHECK(str::Parse<int64>("+1") == 1);
		CHECK(str::Parse<int64>("-1") == -1);
	}

	SECTION("uint8")
	{
		CHECK(str::Parse<uint8>("0") == 0);
		CHECK(str::Parse<uint8>("1") == 1);
		CHECK(str::Parse<uint8>("+1") == 1);
	}

	SECTION("uint16")
	{
		CHECK(str::Parse<uint16>("0") == 0);
		CHECK(str::Parse<uint16>("1") == 1);
		CHECK(str::Parse<uint16>("+1") == 1);
	}

	SECTION("uint32")
	{
		CHECK(str::Parse<uint32>("0") == 0);
		CHECK(str::Parse<uint32>("1") == 1);
		CHECK(str::Parse<uint32>("+1") == 1);
	}

	SECTION("uint64")
	{
		CHECK(str::Parse<uint64>("0") == 0);
		CHECK(str::Parse<uint64>("1") == 1);
		CHECK(str::Parse<uint64>("+1") == 1);
	}

	SECTION("StringView")
	{
		const str::String string = " 1.2345f ";
		const str::StringView stringView = str::StringView(&string[1]).substr(0, 7);
		const auto result = str::Parse<float>(stringView);
		REQUIRE(result);
		CHECK(result == 1.2345f);
	}
}
