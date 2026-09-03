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
		char* m_Array = nullptr;
	};
}

CLASS_TEST_CASE("Create an entity.")
{
	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();

	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entity));
}

CLASS_TEST_CASE("Create multiple entities in the same frame.")
{
	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entityA = buffer.CreateEntity();
	ecs::Entity entityB = buffer.CreateEntity();
	ecs::Entity entityC = buffer.CreateEntity();
	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entityA));
	CHECK(storage.IsAlive(entityB));
	CHECK(storage.IsAlive(entityC));
}

CLASS_TEST_CASE("Create multiple entities in subsequent frames.")
{
	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entityA = buffer.CreateEntity();
	storage.FlushChanges(buffer);

	ecs::Entity entityB = buffer.CreateEntity();
	storage.FlushChanges(buffer);

	ecs::Entity entityC = buffer.CreateEntity();
	storage.FlushChanges(buffer);

	CHECK(storage.IsAlive(entityA));
	CHECK(storage.IsAlive(entityB));
	CHECK(storage.IsAlive(entityC));
}

CLASS_TEST_CASE("Create an entity with a component.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();

	{
		auto& component = buffer.AddComponent<ComponentA>(entity);
		component.m_Bool = true;
	}

	storage.FlushChanges(buffer);

	{
		REQUIRE(storage.IsAlive(entity));
		REQUIRE(storage.HasComponent<ComponentA>(entity));
		auto& component = storage.GetComponent<ComponentA>(entity);
		CHECK(component.m_Bool == true);
	}
}

CLASS_TEST_CASE("Create an entity with multiple components.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	registry.RegisterComponent<ComponentB>();
	registry.RegisterComponent<ComponentC>();
	buffer.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentB>();
	buffer.RegisterComponent<ComponentC>();

	ecs::Entity entity = buffer.CreateEntity();

	{
		auto& componentA = buffer.AddComponent<ComponentA>(entity);
		componentA.m_Bool = true;
		auto& componentB = buffer.AddComponent<ComponentB>(entity);
		componentB.m_Int32 = 1337;
		auto& componentC = buffer.AddComponent<ComponentC>(entity);
		componentC.m_Array = new char[5];
		componentC.m_Array[0] = 'H';
		componentC.m_Array[1] = 'E';
		componentC.m_Array[2] = 'L';
		componentC.m_Array[3] = 'L';
		componentC.m_Array[4] = 'O';
	}

	storage.FlushChanges(buffer);

	{
		REQUIRE(storage.IsAlive(entity));
		REQUIRE(storage.HasComponent<ComponentA>(entity));
		REQUIRE(storage.HasComponent<ComponentB>(entity));
		REQUIRE(storage.HasComponent<ComponentC>(entity));

		auto& componentA = storage.GetComponent<ComponentA>(entity);
		CHECK(componentA.m_Bool == true);

		auto& componentB = storage.GetComponent<ComponentB>(entity);
		CHECK(componentB.m_Int32 == 1337);

		auto& componentC = storage.GetComponent<ComponentC>(entity);
		CHECK(componentC.m_Array[0] == 'H');
		CHECK(componentC.m_Array[1] == 'E');
		CHECK(componentC.m_Array[2] == 'L');
		CHECK(componentC.m_Array[3] == 'L');
		CHECK(componentC.m_Array[4] == 'O');
	}
}

CLASS_TEST_CASE("Create multiple entities with the same component.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entityA = buffer.CreateEntity();
	ecs::Entity entityB = buffer.CreateEntity();
	ecs::Entity entityC = buffer.CreateEntity();

	{
		auto& componentA = buffer.AddComponent<ComponentA>(entityA);
		componentA.m_Bool = false;
		auto& componentB = buffer.AddComponent<ComponentA>(entityB);
		componentB.m_Bool = true;
		auto& componentC = buffer.AddComponent<ComponentA>(entityC);
		componentC.m_Bool = false;
	}

	storage.FlushChanges(buffer);

	{
		REQUIRE(storage.IsAlive(entityA));
		REQUIRE(storage.IsAlive(entityB));
		REQUIRE(storage.IsAlive(entityC));
		REQUIRE(storage.HasComponent<ComponentA>(entityA));
		REQUIRE(storage.HasComponent<ComponentA>(entityB));
		REQUIRE(storage.HasComponent<ComponentA>(entityC));

		auto& componentA = storage.GetComponent<ComponentA>(entityA);
		CHECK(componentA.m_Bool == false);

		auto& componentB = storage.GetComponent<ComponentA>(entityB);
		CHECK(componentB.m_Bool == true);

		auto& componentC = storage.GetComponent<ComponentA>(entityC);
		CHECK(componentC.m_Bool == false);
	}
}

CLASS_TEST_CASE("Create multiple entities with different components.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	registry.RegisterComponent<ComponentB>();
	registry.RegisterComponent<ComponentC>();
	buffer.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentB>();
	buffer.RegisterComponent<ComponentC>();

	ecs::Entity entityA = buffer.CreateEntity();
	ecs::Entity entityB = buffer.CreateEntity();
	ecs::Entity entityC = buffer.CreateEntity();

	{
		auto& componentA = buffer.AddComponent<ComponentA>(entityA);
		componentA.m_Bool = true;
		auto& componentB = buffer.AddComponent<ComponentB>(entityB);
		componentB.m_Int32 = 1337;
		auto& componentC = buffer.AddComponent<ComponentC>(entityC);
		componentC.m_Array = new char[5];
		componentC.m_Array[0] = 'H';
		componentC.m_Array[1] = 'E';
		componentC.m_Array[2] = 'L';
		componentC.m_Array[3] = 'L';
		componentC.m_Array[4] = 'O';
	}

	storage.FlushChanges(buffer);

	{
		REQUIRE(storage.IsAlive(entityA));
		REQUIRE(storage.IsAlive(entityB));
		REQUIRE(storage.IsAlive(entityC));
		REQUIRE(storage.HasComponent<ComponentA>(entityA));
		REQUIRE(storage.HasComponent<ComponentB>(entityB));
		REQUIRE(storage.HasComponent<ComponentC>(entityC));

		auto& componentA = storage.GetComponent<ComponentA>(entityA);
		CHECK(componentA.m_Bool == true);

		auto& componentB = storage.GetComponent<ComponentB>(entityB);
		CHECK(componentB.m_Int32 == 1337);

		auto& componentC = storage.GetComponent<ComponentC>(entityC);
		CHECK(componentC.m_Array[0] == 'H');
		CHECK(componentC.m_Array[1] == 'E');
		CHECK(componentC.m_Array[2] == 'L');
		CHECK(componentC.m_Array[3] == 'L');
		CHECK(componentC.m_Array[4] == 'O');
	}
}

CLASS_TEST_CASE("Destroy an entity.")
{
	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
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
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
	storage.FlushChanges(buffer);

	{
		auto& component = buffer.AddComponent<ComponentA>(entity);
		component.m_Bool = true;
	}

	storage.FlushChanges(buffer);

	{
		REQUIRE(storage.IsAlive(entity));
		REQUIRE(storage.HasComponent<ComponentA>(entity));
		auto& component = storage.GetComponent<ComponentA>(entity);
		CHECK(component.m_Bool == true);
	}
}

CLASS_TEST_CASE("Remove a component from an entity.")
{
	const ecs::ComponentId componentId = ToTypeId<ComponentA, ecs::ComponentTag>();

	ecs::EntityBuffer buffer;
	ecs::TypeRegistry registry;
	ecs::EntityStorage2 storage(registry);
	registry.RegisterComponent<ComponentA>();
	buffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = buffer.CreateEntity();
	buffer.AddComponent<ComponentA>(entity);
	storage.FlushChanges(buffer);

	buffer.RemoveComponent<ComponentA>(entity);
	storage.FlushChanges(buffer);

	REQUIRE(storage.IsAlive(entity));
	CHECK(!storage.HasComponent<ComponentA>(entity));

	storage.FlushChanges(buffer);
}