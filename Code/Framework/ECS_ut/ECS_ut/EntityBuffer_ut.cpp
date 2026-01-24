#include <Catch2/catch.hpp>

#include "ECS/Component.h"
#include "ECS/EntityBuffer.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EntityBuffer. " name, "[ecs::EntityBuffer]")

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA>
	{
		int32 m_Value = 0;
	};

	struct ComponentB final : public ecs::Component<ComponentB>
	{
		int32 m_Value = 0;
	};
}

CLASS_TEST_CASE("The first call to CreateEntity constructs an entity that isn't unassigned.")
{
	ecs::EntityBuffer buffer;
	CHECK(buffer.CreateEntity() != ecs::Entity::Unassigned);
}

CLASS_TEST_CASE("Each call to CreateEntity constructs an entity incrementing the index by 1 each time.")
{
	ecs::EntityBuffer buffer;
	CHECK(buffer.CreateEntity() == ecs::Entity(1));
	CHECK(buffer.CreateEntity() == ecs::Entity(2));
	CHECK(buffer.CreateEntity() == ecs::Entity(3));
	CHECK(buffer.CreateEntity() == ecs::Entity(4));
	CHECK(buffer.CreateEntity() == ecs::Entity(5));
	CHECK(buffer.CreateEntity() == ecs::Entity(6));
}

CLASS_TEST_CASE("Can be constructed with a different ownership id and each entity created also has the same id.")
{
	ecs::EntityBuffer buffer = ecs::EntityBuffer(1);
	CHECK(buffer.CreateEntity() == ecs::Entity(1, 0, 1));
	CHECK(buffer.CreateEntity() == ecs::Entity(2, 0, 1));
	CHECK(buffer.CreateEntity() == ecs::Entity(3, 0, 1));
}

CLASS_TEST_CASE("Entities that have been destroyed are marked in the changes map.")
{
	ecs::EntityBuffer buffer;
	ecs::Entity entity = buffer.CreateEntity();
	buffer.DestroyEntity(entity);

	const auto& changes = buffer.GetChanges();
	REQUIRE(changes.Contains(entity));
	CHECK(changes.Get(entity).m_IsDestroy);
}

CLASS_TEST_CASE("Entities that have been destroyed can be recycled on subsequent creations with an increased version id.")
{
	ecs::EntityBuffer buffer;
	ecs::Entity entity = buffer.CreateEntity();
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 0);

	buffer.DestroyEntity(entity);
	buffer.RecycleEntity(entity);
	entity = buffer.CreateEntity();
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 1);

	buffer.DestroyEntity(entity);
	buffer.RecycleEntity(entity);
	entity = buffer.CreateEntity();
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 2);
}

CLASS_TEST_CASE("Entities are generated with new indexes when there are no recycled handles available.")
{
	ecs::EntityBuffer buffer;
	ecs::Entity entityA = buffer.CreateEntity();
	ecs::Entity entityB = buffer.CreateEntity();
	CHECK(entityA.GetIndex() == 1);
	CHECK(entityA.GetVersion() == 0);
	CHECK(entityB.GetIndex() == 2);
	CHECK(entityB.GetVersion() == 0);

	buffer.DestroyEntity(entityA);
	buffer.DestroyEntity(entityB);
	buffer.RecycleEntity(entityA);
	buffer.RecycleEntity(entityB);

	entityA = buffer.CreateEntity();
	entityB = buffer.CreateEntity();
	CHECK(entityA.GetIndex() == 1);
	CHECK(entityA.GetVersion() == 1);
	CHECK(entityB.GetIndex() == 2);
	CHECK(entityB.GetVersion() == 1);

	ecs::Entity entityC = buffer.CreateEntity();
	CHECK(entityC.GetIndex() == 3);
	CHECK(entityC.GetVersion() == 0);
}

CLASS_TEST_CASE("Registering an event constructs a container for the type.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	REQUIRE(buffer.TryContainerAt<ComponentA>());
}

CLASS_TEST_CASE("Added components are stored inside a component container.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
	auto& component = buffer.AddComponent<ComponentA>(entity);
	component.m_Value = 1;

	const ecs::ComponentContainer<ComponentA>& container = buffer.GetContainerAt<ComponentA>();
	REQUIRE(container.GetCount() == 1);
	CHECK(container.GetValues()[0].m_Value == 1);
}

CLASS_TEST_CASE("Added components are marked on the changes map.")
{
	const ecs::ComponentId typeId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
	buffer.AddComponent<ComponentA>(entity);

	const auto& changes = buffer.GetChanges();
	REQUIRE(changes.Contains(entity));
	CHECK(changes.Get(entity).m_Added.Has(typeId));
}

CLASS_TEST_CASE("Updated components are marked on the changes map.")
{
	const ecs::ComponentId typeId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::Entity entity = buffer.CreateEntity();
	buffer.UpdateComponent<ComponentA>(entity);

	const auto& changes = buffer.GetChanges();
	REQUIRE(changes.Contains(entity));
	CHECK(changes.Get(entity).m_Updated.Has(typeId));
}

CLASS_TEST_CASE("Removed components are marked on the changes map.")
{
	const ecs::ComponentId typeId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::Entity entity = buffer.CreateEntity();
	buffer.RemoveComponent<ComponentA>(entity);

	const auto& changes = buffer.GetChanges();
	REQUIRE(changes.Contains(entity));
	CHECK(changes.Get(entity).m_Removed.Has(typeId));
}

CLASS_TEST_CASE("GetContainers returns all registered containers.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentB>();

	const SparseArray<ecs::ComponentId, ecs::IComponentContainer*>& containers = buffer.GetContainers();
	CHECK(containers.GetCount() == 2);
}

CLASS_TEST_CASE("GetAt returns the container interface that matches the typeId.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	const ecs::ComponentId typeId = ToTypeId<ComponentA, ecs::ComponentTag>();
	CHECK_NOTHROW(buffer.GetContainerAt(typeId));
}

CLASS_TEST_CASE("GetAt returns the container that matches the type.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	CHECK_NOTHROW(buffer.GetContainerAt<ComponentA>());
}

CLASS_TEST_CASE("GetAt can't return a container interface using a typeId if it doesn't exist.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	const ecs::ComponentId typeId = ToTypeId<ComponentA, ecs::ComponentTag>();
	//CHECK_THROWS(buffer.GetContainerAt(typeId));
}

CLASS_TEST_CASE("GetAt can't return a container using a type if it doesn't exist.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	//CHECK_THROWS(buffer.GetContainerAt<ComponentA>());
}

CLASS_TEST_CASE("TryAt returns the container interface that matches the typeId.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	const ecs::ComponentId typeId = ToTypeId<ComponentA, ecs::ComponentTag>();
	CHECK(buffer.TryContainerAt(typeId));
}

CLASS_TEST_CASE("TryAt returns the container that matches the type.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	CHECK(buffer.TryContainerAt<ComponentA>());
}

CLASS_TEST_CASE("TryAt returns nullptr if there are no containers that match the typeId.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	const ecs::ComponentId typeId = ToTypeId<ComponentB, ecs::ComponentTag>();
	CHECK(!buffer.TryContainerAt(typeId));
}

CLASS_TEST_CASE("TryAt returns nullptr if there are no containers that match the type.")
{
	ecs::EntityBuffer buffer;
	buffer.RegisterComponent<ComponentA>();

	CHECK(!buffer.TryContainerAt<ComponentB>());
}