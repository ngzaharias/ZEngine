#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Tuple. " name, "[Tuple]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}