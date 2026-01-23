#include <Catch2/catch.hpp>

#include "ECS/Entity.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::Entity. " name, "[ecs::Entity]")

CLASS_TEST_CASE("Can be default constructed.")
{
	const ecs::Entity entity = {};
	CHECK(entity.GetValue() == 0);
}

CLASS_TEST_CASE("Can be constructed with a uint64.")
{
	constexpr uint64 value = 0x0300000200000001;
	const ecs::Entity entity = ecs::Entity(value);
	CHECK(entity.GetValue() == value);
}

CLASS_TEST_CASE("Can be constructed with another entity.")
{
	const ecs::Entity entityA = ecs::Entity(123456);
	const ecs::Entity entityB = ecs::Entity(entityA);
	CHECK(entityA.GetValue() == entityB.GetValue());
}

CLASS_TEST_CASE("Can be constructed with an index and version.")
{
	const ecs::Entity entity = ecs::Entity(1, 2);
	CHECK(entity.GetValue() == 0x0000000200000001);
}

CLASS_TEST_CASE("Can be constructed with an index, version and ownership.")
{
	const ecs::Entity entity = ecs::Entity(1, 2, 3);
	CHECK(entity.GetValue() == 0x0300000200000001);
}

CLASS_TEST_CASE("Has a static member called Unassigned which is the same as a default constructed entity.")
{
	const ecs::Entity entity = ecs::Entity::Unassigned;
	CHECK(entity.GetValue() == 0);
}

CLASS_TEST_CASE("Has a static member called MainId.")
{
	CHECK(ecs::Entity::MainId == 0);
}

CLASS_TEST_CASE("Has a static member called SyncId.")
{
	CHECK(ecs::Entity::SyncId == 1);
}

CLASS_TEST_CASE("operator bool returns true when called on an entity with a value.")
{
	const ecs::Entity entity = ecs::Entity(1);
	CHECK(entity);
}

CLASS_TEST_CASE("operator bool returns false when called on a default constructed entity.")
{
	const ecs::Entity entity = {};
	CHECK_FALSE(entity);
}

CLASS_TEST_CASE("operator== returns true when comparing two entities with the same value.")
{
	const ecs::Entity entityA = ecs::Entity(1);
	const ecs::Entity entityB = ecs::Entity(1);
	CHECK(entityA == entityB);
}

CLASS_TEST_CASE("operator== returns false when comparing two entities with different values.")
{
	const ecs::Entity entityA = ecs::Entity(1);
	const ecs::Entity entityB = ecs::Entity(2);
	CHECK_FALSE(entityA == entityB);
}

CLASS_TEST_CASE("operator!= returns false when comparing two entities with the same value.")
{
	const ecs::Entity entityA = ecs::Entity(1);
	const ecs::Entity entityB = ecs::Entity(1);
	CHECK_FALSE(entityA != entityB);
}

CLASS_TEST_CASE("operator!= returns true when comparing two entities with different values.")
{
	const ecs::Entity entityA = ecs::Entity(1);
	const ecs::Entity entityB = ecs::Entity(2);
	CHECK(entityA != entityB);
}

CLASS_TEST_CASE("operator< returns true when comparing an entity with a value smaller than the other.")
{
	const ecs::Entity entityA = ecs::Entity(1);
	const ecs::Entity entityB = ecs::Entity(2);
	CHECK(entityA < entityB);
}

CLASS_TEST_CASE("operator< returns false when comparing an entity with the same value.")
{
	const ecs::Entity entityA = ecs::Entity(2);
	const ecs::Entity entityB = ecs::Entity(2);
	CHECK_FALSE(entityA < entityB);
}

CLASS_TEST_CASE("operator< returns false when comparing an entity with a value bigger than the other.")
{
	const ecs::Entity entityA = ecs::Entity(2);
	const ecs::Entity entityB = ecs::Entity(1);
	CHECK_FALSE(entityA < entityB);
}

CLASS_TEST_CASE("IsUnassigned returns true when called on a default constructed entity.")
{
	const ecs::Entity entity;
	CHECK(entity.IsUnassigned());
}

CLASS_TEST_CASE("IsUnassigned returns false when called on an entity constructed with a value.")
{
	const ecs::Entity entity = ecs::Entity(1);
	CHECK_FALSE(entity.IsUnassigned());
}

CLASS_TEST_CASE("GetIndex returns the same value it was constructed with.")
{
	const ecs::Entity entity = ecs::Entity(1, 0);
	CHECK(entity.GetIndex() == 1);
}

CLASS_TEST_CASE("GetVersion returns the same value it was constructed with.")
{
	const ecs::Entity entity = ecs::Entity(0, 1);
	CHECK(entity.GetVersion() == 1);
}

CLASS_TEST_CASE("GetOwnership returns the same value it was constructed with.")
{
	const ecs::Entity entity = ecs::Entity(0, 0, 1);
	CHECK(entity.GetOwnership() == 1);
}

CLASS_TEST_CASE("GetValue returns the same value it was constructed with.")
{
	const ecs::Entity entity = ecs::Entity(123456789);
	CHECK(entity.GetValue() == 123456789);
}

CLASS_TEST_CASE("GetIndex uses bits 0-31.")
{
	const ecs::Entity entity = ecs::Entity(0x00000000FFFFFFFF);
	CHECK(entity.GetIndex() == 0xFFFFFFFF);
}

CLASS_TEST_CASE("GetVersion uses bits 32-55.")
{
	const ecs::Entity entity = ecs::Entity(0x00FFFFFF00000000);
	CHECK(entity.GetVersion() == 0x00FFFFFF);
}

CLASS_TEST_CASE("GetOwnership uses bits 56-63.")
{
	const ecs::Entity entity = ecs::Entity(0xFF00000000000000);
	CHECK(entity.GetOwnership() == 0x000000FF);
}

CLASS_TEST_CASE("GetValue uses all bits.")
{
	const ecs::Entity entity = ecs::Entity(0xFFFFFFFFFFFFFFFF);
	CHECK(entity.GetValue() == 0xFFFFFFFFFFFFFFFF);
}