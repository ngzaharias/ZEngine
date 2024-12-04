#include <Catch2/catch.hpp>

#include "Core/Bitset.h"

TEST_CASE("Bitset. operator[].")
{
	Bitset bitset;

	bitset[0] = true;
	bitset[5] = true;
	bitset[9] = true;

	CHECK(bitset[0] == true);
	CHECK(bitset[5] == true);
	CHECK(bitset[9] == true);
}