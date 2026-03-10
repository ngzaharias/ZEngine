#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("str::StringView. " name, "[str::StringView]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}