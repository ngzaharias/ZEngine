#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("str::Name. " name, "[str::Name]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}