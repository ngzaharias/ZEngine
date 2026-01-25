#include <Catch2/catch.hpp>

#include "ECS/Component.h"
#include "ECS/ComponentContainer.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::ComponentContainer. " name, "[ecs::ComponentContainer]")

namespace
{
	struct Component final : public ecs::Component<Component>
	{
		Component() = default;
		Component(int32 val) : m_Value(val) { }
		int32 m_Value = 1;
	};
}

CLASS_TEST_CASE("Contains returns true if there is an entity with the component.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Set(entity, {});
	CHECK(container.Contains(entity));
}

CLASS_TEST_CASE("Contains returns false if there isn't an entity with the component.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	CHECK(!container.Contains(entity));
}

CLASS_TEST_CASE("Contains can be called on a const reference of the container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	const ecs::ComponentContainer<Component>& reference = container;
	CHECK_NOTHROW(reference.Contains(entity));
}

CLASS_TEST_CASE("Emplace constructs a component in place and adds it to the container using entity as its key.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	auto& component = container.Get(entity);
	CHECK(component.m_Value == 1);
}

CLASS_TEST_CASE("Emplace returns the component after construction so it can be modified further.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	{
		auto& component = container.Emplace(entity);
		component.m_Value = 1337;
	}
	{
		auto& component = container.Get(entity);
		CHECK(component.m_Value == 1337);
	}
}

CLASS_TEST_CASE("Emplace returns a reference to a component but isn't safe to use after a second emplace.")
{
	ecs::Entity entityA(1), entityB(2);
	ecs::ComponentContainer<Component> container;
	auto& componentA = container.Emplace(entityA);
	auto& componentB = container.Emplace(entityB);
	// componentA.m_Value = 1337; // unsafe
	componentB.m_Value = 1337;

	// CHECK(componentA.m_Value == 1); // unsafe
	CHECK(componentB.m_Value == 1337);
}

CLASS_TEST_CASE("Emplace can construct the component using vardic arguments.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity, 1337);

	auto& component = container.Get(entity);
	CHECK(component.m_Value == 1337);
}

CLASS_TEST_CASE("GetCount returns the number of components in the container at any given time.")
{
	ecs::Entity entityA(1), entityB(2), entityC(3);
	ecs::ComponentContainer<Component> container;
	CHECK(container.GetCount() == 0);

	container.Emplace(entityA);
	CHECK(container.GetCount() == 1);

	container.Emplace(entityB);
	container.Emplace(entityC);
	CHECK(container.GetCount() == 3);

	container.Remove(entityB);
	CHECK(container.GetCount() == 2);

	container.RemoveAll();
	CHECK(container.GetCount() == 0);
}

CLASS_TEST_CASE("GetCount can be called on a const reference of the container.")
{
	ecs::ComponentContainer<Component> container;
	const ecs::ComponentContainer<Component>& reference = container;
	CHECK(reference.GetCount() == 0);
}

CLASS_TEST_CASE("Get returns the component from the container that matches the entity.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	auto& component = container.Get(entity);
	CHECK(component.m_Value == 1);
}

CLASS_TEST_CASE("Get crashes when called with an entity that doesn't exist in the container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	//CHECK_THROWS(container.Get(entity));
}

CLASS_TEST_CASE("Get returns a modifiable component from the container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);
	{
		auto& component = container.Get(entity);
		component.m_Value = 1337;
	}
	{
		auto& component = container.Get(entity);
		CHECK(component.m_Value == 1337);
	}
}

CLASS_TEST_CASE("Get can be called on a const container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	const ecs::ComponentContainer<Component>& reference = container;
	auto& component = reference.Get(entity);
	CHECK(component.m_Value == 1);
}

CLASS_TEST_CASE("Get returns a const reference to a component when called on a const container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	const ecs::ComponentContainer<Component>& reference = container;
	auto& component = reference.Get(entity);
	//CHECK_THROWS(component.m_Value = 1337); // compile error
}

CLASS_TEST_CASE("GetAll returns all values in the container.")
{
	ecs::Entity entityA(1), entityB(2), entityC(3);
	ecs::ComponentContainer<Component> container;
	container.Emplace(entityA, 1);
	container.Emplace(entityB, 2);
	container.Emplace(entityC, 3);

	Array<Component>& values = container.GetAll();
	REQUIRE(values.GetCount() == 3);
	CHECK(values[0].m_Value == 1);
	CHECK(values[1].m_Value == 2);
	CHECK(values[2].m_Value == 3);
}

CLASS_TEST_CASE("GetAll doesn't guarentee they are returned in the order they were added in.")
{
	ecs::Entity entityA(1), entityB(2), entityC(3);
	ecs::ComponentContainer<Component> container;
	container.Emplace(entityA, 1);
	container.Emplace(entityB, 2);
	container.Emplace(entityC, 3);
	container.Remove(entityA);

	Array<Component>& values = container.GetAll();
	REQUIRE(values.GetCount() == 2);
	CHECK(values[0].m_Value == 3);
	CHECK(values[1].m_Value == 2);
}

CLASS_TEST_CASE("GetAll returns a const reference of values when called on a const container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	const ecs::ComponentContainer<Component>& reference = container;
	auto& values = reference.GetAll();
	//CHECK_THROWS(values.Emplace(entity)); // compile error
}

CLASS_TEST_CASE("Try returns the component from the container that matches the entity.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	auto* component = container.Try(entity);
	CHECK(component->m_Value == 1);
}

CLASS_TEST_CASE("Try returns nullptr when called with an entity that doesn't exist in the container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	CHECK(!container.Try(entity));
}

CLASS_TEST_CASE("Try returns a modifiable component from the container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);
	{
		auto* component = container.Try(entity);
		component->m_Value = 1337;
	}
	{
		auto* component = container.Try(entity);
		CHECK(component->m_Value == 1337);
	}
}

CLASS_TEST_CASE("Try can be called on a const container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	const ecs::ComponentContainer<Component>& reference = container;
	auto* component = reference.Try(entity);
	CHECK(component->m_Value == 1);
}

CLASS_TEST_CASE("Try returns a const reference to a component when called on a const container.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> container;
	container.Emplace(entity);

	const ecs::ComponentContainer<Component>& reference = container;
	auto* component = reference.Try(entity);
	//CHECK_THROWS(component->m_Value = 1337); // compile error
}

CLASS_TEST_CASE("Set.")
{
}

CLASS_TEST_CASE("Move transfers a single entity and component from one container to another.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> containerA;
	ecs::ComponentContainer<Component> containerB;
	containerA.Emplace(entity, 1);
	containerA.Move(entity, containerB);

	const auto& values = containerB.GetAll();
	REQUIRE(values.GetCount() == 1);
	CHECK(values[0].m_Value == 1);
}

CLASS_TEST_CASE("Move removes the entity and component after they've been moved.")
{
	ecs::Entity entityA(1), entityB(2);
	ecs::ComponentContainer<Component> containerA;
	ecs::ComponentContainer<Component> containerB;
	containerA.Emplace(entityA, 1);
	containerA.Emplace(entityB, 2);
	containerA.Move(entityB, containerB);

	const auto& valueA = containerA.Get(entityA);
	const auto& valueB = containerB.Get(entityB);
	CHECK(valueA.m_Value == 1);
	CHECK(valueB.m_Value == 2);
}

CLASS_TEST_CASE("MoveAll transfers all entities and components from one container to another.")
{
	ecs::Entity entityA(1), entityB(2);
	ecs::ComponentContainer<Component> containerA;
	ecs::ComponentContainer<Component> containerB;
	containerA.Emplace(entityA, 1);
	containerA.Emplace(entityB, 2);
	containerA.MoveAll(containerB);

	const auto& values = containerB.GetAll();
	REQUIRE(values.GetCount() == 2);
	CHECK(values[0].m_Value == 1);
	CHECK(values[1].m_Value == 2);
}

CLASS_TEST_CASE("MoveAll removes the entities and components after they've been moved.")
{
	ecs::Entity entity;
	ecs::ComponentContainer<Component> containerA;
	ecs::ComponentContainer<Component> containerB;
	containerA.Emplace(entity, 1);
	containerA.Emplace(entity, 2);
	containerA.MoveAll(containerB);

	REQUIRE(containerA.GetCount() == 0);
}

CLASS_TEST_CASE("Remove will remove an entity and its component from the container.")
{
	ecs::Entity entityA(1), entityB(2);
	ecs::ComponentContainer<Component> container;
	container.Emplace(entityA, 1);
	container.Emplace(entityB, 2);
	container.Remove(entityA);

	CHECK_NOTHROW(container.Get(entityB));
}

CLASS_TEST_CASE("RemoveAll will remove every entity and component from the container.")
{
	ecs::Entity entityA(1), entityB(2);
	ecs::ComponentContainer<Component> container;
	container.Emplace(entityA, 1);
	container.Emplace(entityB, 2);
	container.RemoveAll();

	CHECK(container.GetCount() == 0);
}