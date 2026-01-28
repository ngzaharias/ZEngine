#include <Catch2/catch.hpp>

#include "ECS/Component.h"
#include "ECS/EntityStorage.h"
#include "ECS/QueryRegistry.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EntityStorage. " name, "[ecs::EntityStorage]")

namespace
{
	struct Component final : public ecs::Component<Component>
	{
		Component() = default;
		Component(int32 val) : m_Value(val) { }
		int32 m_Value = 1;
	};
}

CLASS_TEST_CASE("IsAlive returns false on an entity that was just created.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	ecs::Entity entity = storage.CreateEntity();
	CHECK(!storage.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns true on an entity that was created after the changes have been flushed.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	ecs::Entity entity = storage.CreateEntity();

	storage.FlushChanges();
	CHECK(storage.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns true on an entity that was just destroyed.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	ecs::Entity entity = storage.CreateEntity();
	storage.FlushChanges();

	storage.DestroyEntity(entity);
	CHECK(storage.IsAlive(entity));
}

CLASS_TEST_CASE("IsAlive returns false on an entity that was destroyed after the changes have been flushed.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	ecs::Entity entity = storage.CreateEntity();
	storage.FlushChanges();

	storage.DestroyEntity(entity);
	storage.FlushChanges();
	CHECK(!storage.IsAlive(entity));
}

CLASS_TEST_CASE("CreateEntity constructs a valid entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	ecs::Entity entity = storage.CreateEntity();
	CHECK(!entity.IsUnassigned());
}

CLASS_TEST_CASE("DestroyEntity marks an entity to be destroyed.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	ecs::Entity entity = storage.CreateEntity();
	storage.FlushChanges();
	storage.DestroyEntity(entity);
}

CLASS_TEST_CASE("RegisterComponent allows components to be registered with the storage.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();
}

CLASS_TEST_CASE("AddComponent can add a component to an entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();
	CHECK(storage.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("AddComponent can construct a component using vardic arguments.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	const auto& componentA = storage.AddComponent<Component>(entity, 1337);
	CHECK(componentA.m_Value == 1337);
}

CLASS_TEST_CASE("AddComponent will crash the game if called twice on the same entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	//CHECK_THROWS(world.AddComponent<Component>(entity));
	//CHECK_THROWS(world.FlushChanges());
}

CLASS_TEST_CASE("AddComponent can be called again after the component was removed.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();

	storage.RemoveComponent<Component>(entity);
	storage.FlushChanges();

	storage.AddComponent<Component>(entity);
	storage.FlushChanges();

	CHECK(storage.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("RemoveComponent will remove the component from an entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();

	CHECK_NOTHROW(storage.RemoveComponent<Component>(entity));
	CHECK_NOTHROW(storage.FlushChanges());
}

CLASS_TEST_CASE("RemoveComponent will crash the game if called on an entity that doesn't have the component.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	//CHECK_THROWS(storage.RemoveComponent<Component>(entity));
	//CHECK_THROWS(storage.FlushChanges());
}

CLASS_TEST_CASE("HasComponent returns true if a component has been added to an entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();

	CHECK(storage.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns false if a component hasn't been added to an entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	CHECK(!storage.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns true if a component was removed but the world hasn't updated yet.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();
	storage.RemoveComponent<Component>(entity);
	CHECK(storage.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("HasComponent returns false if a component was added but the world hasn't updated yet.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	CHECK(!storage.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("GetComponent returns a component that is added to an entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();
	auto& component = storage.GetComponent<Component>(entity);
	CHECK(component.m_Value == 1);
}

CLASS_TEST_CASE("GetComponent returns a component that can be modified.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();
	auto& component = storage.GetComponent<Component>(entity);
	component.m_Value = 1337;
	CHECK(component.m_Value == 1337);
}

CLASS_TEST_CASE("GetComponent crashes when the component hasn't been added to the entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	//CHECK_THROWS(world.GetComponent<Component>(entity));
}

CLASS_TEST_CASE("TryComponent returns a component that is added to an entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();
	auto* component = storage.TryComponent<Component>(entity);
	CHECK(component->m_Value == 1);
}

CLASS_TEST_CASE("TryComponent returns a component that can be modified.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	storage.AddComponent<Component>(entity);
	storage.FlushChanges();
	auto* component = storage.TryComponent<Component>(entity);
	component->m_Value = 1337;
	CHECK(component->m_Value == 1337);
}

CLASS_TEST_CASE("TryComponent returns nullptr when the component hasn't been added to the entity.")
{
	ecs::QueryRegistry registry;
	ecs::EntityStorage storage = ecs::EntityStorage(registry);
	storage.RegisterComponent<Component>();

	ecs::Entity entity = storage.CreateEntity();
	CHECK(!storage.TryComponent<Component>(entity));
}