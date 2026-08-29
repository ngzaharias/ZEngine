#include "CorePCH.h"

#include "Core/SortHelpers.h"
#include "Core/StringView.h"

#define HELPER_TEST_CASE(name) TEST_CASE("sort::" name, "[sort]")

HELPER_TEST_CASE("AlphaNumeric. Characters are sorted alphabetically.")
{
	CHECK(sort::AlphaNumeric("a", "b") == -1);
	CHECK(sort::AlphaNumeric("A", "B") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Characters are sorted alphabetically even when mixing cases.")
{
	CHECK(sort::AlphaNumeric("a", "B") == -1);
	CHECK(sort::AlphaNumeric("A", "b") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Characters are sorted alphabetically regardless the order they are provided.")
{
	CHECK(sort::AlphaNumeric("B", "A") == 1);
	CHECK(sort::AlphaNumeric("B", "a") == 1);
}

HELPER_TEST_CASE("AlphaNumeric. Characters are sorted numerically.")
{
	CHECK(sort::AlphaNumeric("1", "2") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Characters are sorted numerically regardless the order they are provided.")
{
	CHECK(sort::AlphaNumeric("2", "1") == 1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted alphabetically.")
{
	CHECK(sort::AlphaNumeric("alpha", "beta") == -1);
	CHECK(sort::AlphaNumeric("ALPHA", "BETA") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted alphabetically from left to right.")
{
	CHECK(sort::AlphaNumeric("alpha", "alphb") == -1);
	CHECK(sort::AlphaNumeric("ALPHA", "ALPHB") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted alphabetically even when mixing cases.")
{
	CHECK(sort::AlphaNumeric("alpha", "BETA") == -1);
	CHECK(sort::AlphaNumeric("ALPHA", "beta") == -1);
	CHECK(sort::AlphaNumeric("aLpHa", "BeTa") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted alphabetically regardless the order they are provided.")
{
	CHECK(sort::AlphaNumeric("beta", "alpha") == 1);
	CHECK(sort::AlphaNumeric("BETA", "ALPHA") == 1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted numerically.")
{
	CHECK(sort::AlphaNumeric("100", "200") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted numerically including leading 0s.")
{
	CHECK(sort::AlphaNumeric("02", "10") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted numerically and not left to right.")
{
	CHECK(sort::AlphaNumeric("2", "10") == -1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted numerically regardless the order they are provided.")
{
	CHECK(sort::AlphaNumeric("200", "100") == 1);
}

HELPER_TEST_CASE("AlphaNumeric. Strings are sorted alphabetically and numerically when mixed together.")
{
	CHECK(sort::AlphaNumeric("file02", "file10") == -1);
}