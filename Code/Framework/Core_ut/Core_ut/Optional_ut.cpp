#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Optional. " name, "[Optional]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}