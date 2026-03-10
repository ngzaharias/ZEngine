#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("str::" name, "[str]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}