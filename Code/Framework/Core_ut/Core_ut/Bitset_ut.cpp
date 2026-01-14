#include "CorePCH.h"

#include "Core/Bitset.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Bitset. " name, "[Bitset]")

CLASS_TEST_CASE("operator[].")
{
	Bitset bitset;

	bitset[0] = true;
	bitset[5] = true;
	bitset[9] = true;

	CHECK(bitset[0] == true);
	CHECK(bitset[5] == true);
	CHECK(bitset[9] == true);
}