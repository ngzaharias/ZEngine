#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("TypeName. " name, "[TypeName]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}