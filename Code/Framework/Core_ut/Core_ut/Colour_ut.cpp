#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Colour. " name, "[Colour]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}