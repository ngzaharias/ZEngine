#include <Catch2/catch.hpp>

#include "ECS/EntityBuffer.h"
#include "ECS/EntityStorage2.h"
#include "ECS/QueryRegistry.h"
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
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = entityBuffer.CreateEntity();

	entityStorage.FlushChanges(entityBuffer);

	CHECK(entityStorage.IsAlive(entity));
}

CLASS_TEST_CASE("Create multiple entities in the same frame.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entityA = entityBuffer.CreateEntity();
	ecs::Entity entityB = entityBuffer.CreateEntity();
	ecs::Entity entityC = entityBuffer.CreateEntity();
	entityStorage.FlushChanges(entityBuffer);

	CHECK(entityStorage.IsAlive(entityA));
	CHECK(entityStorage.IsAlive(entityB));
	CHECK(entityStorage.IsAlive(entityC));
}

CLASS_TEST_CASE("Create multiple entities in subsequent frames.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entityA = entityBuffer.CreateEntity();
	entityStorage.FlushChanges(entityBuffer);

	ecs::Entity entityB = entityBuffer.CreateEntity();
	entityStorage.FlushChanges(entityBuffer);

	ecs::Entity entityC = entityBuffer.CreateEntity();
	entityStorage.FlushChanges(entityBuffer);

	CHECK(entityStorage.IsAlive(entityA));
	CHECK(entityStorage.IsAlive(entityB));
	CHECK(entityStorage.IsAlive(entityC));
}

CLASS_TEST_CASE("Create an entity with a component.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = entityBuffer.CreateEntity();

	{
		auto& component = entityBuffer.AddComponent<ComponentA>(entity);
		component.m_Bool = true;
	}

	entityStorage.FlushChanges(entityBuffer);

	{
		REQUIRE(entityStorage.IsAlive(entity));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entity));
		auto& component = entityStorage.GetComponent<ComponentA>(entity);
		CHECK(component.m_Bool == true);
	}
}

CLASS_TEST_CASE("Create an entity with multiple components.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	typeRegistry.RegisterComponent<ComponentB>();
	typeRegistry.RegisterComponent<ComponentC>();
	entityBuffer.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentB>();
	entityBuffer.RegisterComponent<ComponentC>();

	ecs::Entity entity = entityBuffer.CreateEntity();

	{
		auto& componentA = entityBuffer.AddComponent<ComponentA>(entity);
		componentA.m_Bool = true;
		auto& componentB = entityBuffer.AddComponent<ComponentB>(entity);
		componentB.m_Int32 = 1337;
		auto& componentC = entityBuffer.AddComponent<ComponentC>(entity);
		componentC.m_Array = new char[5];
		componentC.m_Array[0] = 'H';
		componentC.m_Array[1] = 'E';
		componentC.m_Array[2] = 'L';
		componentC.m_Array[3] = 'L';
		componentC.m_Array[4] = 'O';
	}

	entityStorage.FlushChanges(entityBuffer);

	{
		REQUIRE(entityStorage.IsAlive(entity));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entity));
		REQUIRE(entityStorage.HasComponent<ComponentB>(entity));
		REQUIRE(entityStorage.HasComponent<ComponentC>(entity));

		auto& componentA = entityStorage.GetComponent<ComponentA>(entity);
		CHECK(componentA.m_Bool == true);

		auto& componentB = entityStorage.GetComponent<ComponentB>(entity);
		CHECK(componentB.m_Int32 == 1337);

		auto& componentC = entityStorage.GetComponent<ComponentC>(entity);
		CHECK(componentC.m_Array[0] == 'H');
		CHECK(componentC.m_Array[1] == 'E');
		CHECK(componentC.m_Array[2] == 'L');
		CHECK(componentC.m_Array[3] == 'L');
		CHECK(componentC.m_Array[4] == 'O');
	}
}

CLASS_TEST_CASE("Create multiple entities with the same component.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entityA = entityBuffer.CreateEntity();
	ecs::Entity entityB = entityBuffer.CreateEntity();
	ecs::Entity entityC = entityBuffer.CreateEntity();

	{
		auto& componentA = entityBuffer.AddComponent<ComponentA>(entityA);
		componentA.m_Bool = false;
		auto& componentB = entityBuffer.AddComponent<ComponentA>(entityB);
		componentB.m_Bool = true;
		auto& componentC = entityBuffer.AddComponent<ComponentA>(entityC);
		componentC.m_Bool = false;
	}

	entityStorage.FlushChanges(entityBuffer);

	{
		REQUIRE(entityStorage.IsAlive(entityA));
		REQUIRE(entityStorage.IsAlive(entityB));
		REQUIRE(entityStorage.IsAlive(entityC));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entityA));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entityB));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entityC));

		auto& componentA = entityStorage.GetComponent<ComponentA>(entityA);
		CHECK(componentA.m_Bool == false);

		auto& componentB = entityStorage.GetComponent<ComponentA>(entityB);
		CHECK(componentB.m_Bool == true);

		auto& componentC = entityStorage.GetComponent<ComponentA>(entityC);
		CHECK(componentC.m_Bool == false);
	}
}

CLASS_TEST_CASE("Create multiple entities with different components.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	typeRegistry.RegisterComponent<ComponentB>();
	typeRegistry.RegisterComponent<ComponentC>();
	entityBuffer.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentB>();
	entityBuffer.RegisterComponent<ComponentC>();

	ecs::Entity entityA = entityBuffer.CreateEntity();
	ecs::Entity entityB = entityBuffer.CreateEntity();
	ecs::Entity entityC = entityBuffer.CreateEntity();

	{
		auto& componentA = entityBuffer.AddComponent<ComponentA>(entityA);
		componentA.m_Bool = true;
		auto& componentB = entityBuffer.AddComponent<ComponentB>(entityB);
		componentB.m_Int32 = 1337;
		auto& componentC = entityBuffer.AddComponent<ComponentC>(entityC);
		componentC.m_Array = new char[5];
		componentC.m_Array[0] = 'H';
		componentC.m_Array[1] = 'E';
		componentC.m_Array[2] = 'L';
		componentC.m_Array[3] = 'L';
		componentC.m_Array[4] = 'O';
	}

	entityStorage.FlushChanges(entityBuffer);

	{
		REQUIRE(entityStorage.IsAlive(entityA));
		REQUIRE(entityStorage.IsAlive(entityB));
		REQUIRE(entityStorage.IsAlive(entityC));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entityA));
		REQUIRE(entityStorage.HasComponent<ComponentB>(entityB));
		REQUIRE(entityStorage.HasComponent<ComponentC>(entityC));

		auto& componentA = entityStorage.GetComponent<ComponentA>(entityA);
		CHECK(componentA.m_Bool == true);

		auto& componentB = entityStorage.GetComponent<ComponentB>(entityB);
		CHECK(componentB.m_Int32 == 1337);

		auto& componentC = entityStorage.GetComponent<ComponentC>(entityC);
		CHECK(componentC.m_Array[0] == 'H');
		CHECK(componentC.m_Array[1] == 'E');
		CHECK(componentC.m_Array[2] == 'L');
		CHECK(componentC.m_Array[3] == 'L');
		CHECK(componentC.m_Array[4] == 'O');
	}
}

CLASS_TEST_CASE("Destroy an entity.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = entityBuffer.CreateEntity();
	entityStorage.FlushChanges(entityBuffer);

	entityBuffer.DestroyEntity(entity);
	entityStorage.FlushChanges(entityBuffer);

	CHECK(!entityStorage.IsAlive(entity));
}

CLASS_TEST_CASE("Add a component to an entity.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();

	ecs::Entity entity = entityBuffer.CreateEntity();
	entityStorage.FlushChanges(entityBuffer);

	{
		auto& component = entityBuffer.AddComponent<ComponentA>(entity);
		component.m_Bool = true;
	}

	entityStorage.FlushChanges(entityBuffer);

	{
		REQUIRE(entityStorage.IsAlive(entity));
		REQUIRE(entityStorage.HasComponent<ComponentA>(entity));
		auto& component = entityStorage.GetComponent<ComponentA>(entity);
		CHECK(component.m_Bool == true);
	}
}

CLASS_TEST_CASE("Remove a component from an entity.")
{
	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();
	queryRegistry.Initialise();

	ecs::Entity entity = entityBuffer.CreateEntity();
	entityBuffer.AddComponent<ComponentA>(entity);
	entityStorage.FlushChanges(entityBuffer);

	entityBuffer.RemoveComponent<ComponentA>(entity);
	entityStorage.FlushChanges(entityBuffer);

	REQUIRE(entityStorage.IsAlive(entity));
	CHECK(!entityStorage.HasComponent<ComponentA>(entity));

	entityStorage.FlushChanges(entityBuffer);
}

CLASS_TEST_CASE("Table is registered with Query when it is created.")
{
	using Query = ecs::query::Include<ComponentA>;
	static const ecs::QueryId queryId = ecs::QueryProxy<Query>::Id();

	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();
	queryRegistry.Initialise();

	{
		const ecs::QueryGroupB& group = queryRegistry.GetGroupB(queryId);
		REQUIRE(group.GetCount() == 0);
	}

	ecs::Entity entity = entityBuffer.CreateEntity();
	entityBuffer.AddComponent<ComponentA>(entity);
	entityStorage.FlushChanges(entityBuffer);

	{
		const ecs::QueryGroupB& group = queryRegistry.GetGroupB(queryId);
		CHECK(group.GetCount() == 1);
	}
}

CLASS_TEST_CASE("Table is unregistered with Query when it is destroyed.")
{
	using Query = ecs::query::Include<ComponentA>;
	static const ecs::QueryId queryId = ecs::QueryProxy<Query>::Id();

	ecs::EntityBuffer entityBuffer;
	ecs::TypeRegistry typeRegistry;
	ecs::QueryRegistry queryRegistry;
	ecs::EntityStorage2 entityStorage(queryRegistry, typeRegistry);
	typeRegistry.RegisterComponent<ComponentA>();
	entityBuffer.RegisterComponent<ComponentA>();
	queryRegistry.Initialise();

	ecs::Entity entity = entityBuffer.CreateEntity();
	entityBuffer.AddComponent<ComponentA>(entity);
	entityStorage.FlushChanges(entityBuffer);

	{
		const ecs::QueryGroupB& group = queryRegistry.GetGroupB(queryId);
		REQUIRE(group.GetCount() == 1);
	}

	entityBuffer.DestroyEntity(entity);
	entityStorage.FlushChanges(entityBuffer);

	{
		const ecs::QueryGroupB& group = queryRegistry.GetGroupB(queryId);
		CHECK(group.GetCount() == 0);
	}
}