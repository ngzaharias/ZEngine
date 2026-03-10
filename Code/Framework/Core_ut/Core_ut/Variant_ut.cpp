#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Variant. " name, "[Variant]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}