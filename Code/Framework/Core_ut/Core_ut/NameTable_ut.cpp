#include "CorePCH.h"

#define CLASS_TEST_CASE(name) TEST_CASE("str::NameTable. " name, "[str::NameTable]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}