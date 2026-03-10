#include <Catch2/catch.hpp>

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::ReplicationHost. " name, "[ecs::ReplicationHost]")

CLASS_TEST_CASE("Test.")
{
	CHECK(false);
}