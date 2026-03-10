#include <Catch2/catch.hpp>

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::SingletonContainer. " name, "[ecs::SingletonContainer]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}