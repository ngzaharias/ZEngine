#include "CorePCH.h"

#define HELPER_TEST_CASE(name) TEST_CASE("random::" name, "[random]")

HELPER_TEST_CASE("Test.")
{
	CHECK(false);
}