#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("str::String. " name, "[str::String]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}