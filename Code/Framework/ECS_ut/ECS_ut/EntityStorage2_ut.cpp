#include <Catch2/catch.hpp>

#include "ECS/EntityBuffer.h"
#include "ECS/EntityStorage2.h"
#include "ECS/TypeRegistry.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::EntityStorage2. " name, "[ecs::EntityStorage2]")

namespace
{
	struct ComponentA : public ecs::Component
	{
		bool m_Bool = false;
	};

	struct ComponentB : public ecs::Component
	{
		int32 m_Int32 = 0;
	};

	struct ComponentC : public ecs::Component
	{
		~ComponentC()
		{
			delete[] m_Array;
		}

		char* m_Array = nullptr;
	};
}

CLASS_TEST_CASE("Create an entity.")
{
	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();

	ecs::EntityStorage2 storage(registry);
	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entity));
}

CLASS_TEST_CASE("Create an entity with a component.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
	buffer.AddComponent<ComponentA>(entity);

	ecs::EntityStorage2 storage(registry);
	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entity));
	CHECK(storage.HasComponent<ComponentA>(entity));
}

CLASS_TEST_CASE("Destroy an entity.")
{
	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();

	ecs::EntityStorage2 storage(registry);
	storage.FlushChanges(buffer);

	buffer.DestroyEntity(entity);
	storage.FlushChanges(buffer);

	CHECK(!storage.IsAlive(entity));
}

CLASS_TEST_CASE("Add a component to an entity.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();

	ecs::EntityStorage2 storage(registry);
	storage.FlushChanges(buffer);

	buffer.AddComponent<ComponentA>(entity);

	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entity));
	CHECK(storage.HasComponent<ComponentA>(entity));
}

CLASS_TEST_CASE("Remove a component from an entity.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
	buffer.AddComponent<ComponentA>(entity);

	ecs::EntityStorage2 storage(registry);
	storage.FlushChanges(buffer);

	buffer.RemoveComponent<ComponentA>(entity);

	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entity));
	CHECK(!storage.HasComponent<ComponentA>(entity));
}