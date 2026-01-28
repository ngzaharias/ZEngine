#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ECS/QueryRegistry.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::WorldView. " name, "[ecs::WorldView]")

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> {};
	struct ComponentB final : public ecs::Component<ComponentB> {};

	struct SingletonA final : public ecs::Singleton<SingletonA> {};
	struct SingletonB final : public ecs::Singleton<SingletonB> {};

	class ResourceA { };
	class ResourceB { };

	struct RAIIHelper
	{
		RAIIHelper()
		{
			m_EntityWorld.RegisterComponent<ComponentA>();
			m_EntityWorld.RegisterComponent<ComponentB>();
			m_EntityWorld.RegisterSingleton<SingletonA>();
			m_EntityWorld.RegisterSingleton<SingletonB>();
			m_EntityWorld.RegisterResource<ResourceA>(m_ResourceA);
			m_EntityWorld.RegisterResource<ResourceB>(m_ResourceB);
			m_EntityWorld.Initialise();

			m_EntityA = m_EntityWorld.CreateEntity();
			m_EntityB = m_EntityWorld.CreateEntity();
			m_EntityC = m_EntityWorld.CreateEntity();
			m_EntityWorld.Update({});
		}

		ecs::EntityWorld m_EntityWorld = {};
		ecs::Entity m_EntityA = {};
		ecs::Entity m_EntityB = {};
		ecs::Entity m_EntityC = {};
		ResourceA m_ResourceA = {};
		ResourceB m_ResourceB = {};
	};
}

CLASS_TEST_CASE("CreateEntity.")
{
	ecs::EntityWorld entityWorld;
	ecs::WorldView world = entityWorld.WorldView<ecs::WorldView>();

	CHECK(world.CreateEntity() == ecs::Entity(0));
	CHECK(world.CreateEntity() == ecs::Entity(1));
	CHECK(world.CreateEntity() == ecs::Entity(2));
	CHECK(world.CreateEntity() == ecs::Entity(3));
	CHECK(world.CreateEntity() == ecs::Entity(4));
	CHECK(world.CreateEntity() == ecs::Entity(5));
}

CLASS_TEST_CASE("DestroyEntity.")
{
	using WorldView = ecs::WorldView;

	ecs::EntityWorld entityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	ecs::Entity entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 0);
	CHECK(entity.GetVersion() == 0);

	worldView.DestroyEntity(entity);
	entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 0);

	worldView.DestroyEntity(entity);
	entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 2);
	CHECK(entity.GetVersion() == 0);

	entity = worldView.CreateEntity();
	CHECK(entity.GetIndex() == 0);
	CHECK(entity.GetVersion() == 1);
}

CLASS_TEST_CASE("IsAlive.")
{
	using WorldView = ecs::WorldView;

	ecs::EntityWorld entityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	ecs::Entity entity;
	CHECK(!worldView.IsAlive(entity));
	entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(worldView.IsAlive(entity));
	worldView.DestroyEntity(entity);
	entityWorld.Update({});

	CHECK(!worldView.IsAlive(entity));
}

CLASS_TEST_CASE("AddComponent.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();
	ecs::Entity entity = worldView.CreateEntity();

	CHECK_NOTHROW(worldView.AddComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(worldView.HasComponent<ComponentA>(entity));
	CHECK(!worldView.HasComponent<ComponentB>(entity));
}

CLASS_TEST_CASE("RemoveComponent.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	ecs::Entity entity = worldView.CreateEntity();
	worldView.AddComponent<ComponentA>(entity);
	worldView.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.RemoveComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(!worldView.HasComponent<ComponentA>(entity));
	CHECK(worldView.HasComponent<ComponentB>(entity));
}

CLASS_TEST_CASE("HasComponent.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	CHECK(!worldView.HasComponent<ComponentA>(raii.m_EntityA));
	CHECK(!worldView.HasComponent<ComponentB>(raii.m_EntityA));

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityA);
	entityWorld.Update({});

	CHECK(worldView.HasComponent<ComponentA>(raii.m_EntityA));
	CHECK(worldView.HasComponent<ComponentB>(raii.m_EntityA));
}

CLASS_TEST_CASE("ReadComponent.")
{
	using WorldView = ecs::WorldView
		::Read<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityA);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.ReadComponent<ComponentA>(raii.m_EntityA));
	CHECK_NOTHROW(worldView.ReadComponent<ComponentB>(raii.m_EntityA));
}

CLASS_TEST_CASE("WriteComponent.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityA);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.WriteComponent<ComponentA>(raii.m_EntityA));
	CHECK_NOTHROW(worldView.WriteComponent<ComponentB>(raii.m_EntityA));
}

CLASS_TEST_CASE("ReadSingleton.")
{
	using WorldView = ecs::WorldView
		::Read<SingletonA, SingletonB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	CHECK_NOTHROW(worldView.ReadSingleton<SingletonA>());
	CHECK_NOTHROW(worldView.ReadSingleton<SingletonB>());
}

CLASS_TEST_CASE("WriteSingleton.")
{
	using WorldView = ecs::WorldView
		::Write<SingletonA, SingletonB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	CHECK_NOTHROW(worldView.WriteSingleton<SingletonA>());
	CHECK_NOTHROW(worldView.WriteSingleton<SingletonB>());
}

CLASS_TEST_CASE("ReadResource.")
{
	using WorldView = ecs::WorldView
		::Read<ResourceA, ResourceB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	CHECK_NOTHROW(worldView.ReadResource<ResourceA>());
	CHECK_NOTHROW(worldView.ReadResource<ResourceB>());
}

CLASS_TEST_CASE("WriteResource.")
{
	using WorldView = ecs::WorldView
		::Write<ResourceA, ResourceB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	CHECK_NOTHROW(worldView.WriteResource<ResourceA>());
	CHECK_NOTHROW(worldView.WriteResource<ResourceB>());
}

CLASS_TEST_CASE("Components that are added to an entity are present in the added query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	// Query is active on the next frame.
	CHECK(worldView.Count<ecs::query::Added<ComponentA>>() == 2);
	CHECK(worldView.Count<ecs::query::Added<ComponentB>>() == 2);
	CHECK(worldView.Count<ecs::query::Added<ComponentA, ComponentB>>() == 1);

	// Query is cleared on the frame after.
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Added<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Added<ComponentB>>() == 0);
	CHECK(worldView.Count<ecs::query::Added<ComponentA, ComponentB>>() == 0);
}

CLASS_TEST_CASE("Components that are removed from an entity are present in the removed query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityB);
	worldView.RemoveComponent<ComponentA>(raii.m_EntityC);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	// Query is active on the next frame.
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 2);
	CHECK(worldView.Count<ecs::query::Removed<ComponentB>>() == 2);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA, ComponentB>>() == 1);

	entityWorld.Update({});

	// Query is cleared on the frame after.
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentB>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA, ComponentB>>() == 0);
}

CLASS_TEST_CASE("Updated Query isn't triggered when using ReadComponent.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	worldView.ReadComponent<ComponentA>(raii.m_EntityA);
	worldView.ReadComponent<ComponentB>(raii.m_EntityB);
	worldView.ReadComponent<ComponentA>(raii.m_EntityC);
	worldView.ReadComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Updated<ComponentB>>() == 0);
	CHECK(worldView.Count<ecs::query::Updated<ComponentA, ComponentB>>() == 0);
}

CLASS_TEST_CASE("Updated Query isn't triggered when using WriteComponent.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	worldView.WriteComponent<ComponentA>(raii.m_EntityA);
	worldView.WriteComponent<ComponentB>(raii.m_EntityB);
	worldView.WriteComponent<ComponentA>(raii.m_EntityC);
	worldView.WriteComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	// Query is active on the next frame.
	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 2);
	CHECK(worldView.Count<ecs::query::Updated<ComponentB>>() == 2);
	CHECK(worldView.Count<ecs::query::Updated<ComponentA, ComponentB>>() == 1);

	entityWorld.Update({});

	// Query is cleared on the frame after.
	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Updated<ComponentB>>() == 0);
	CHECK(worldView.Count<ecs::query::Updated<ComponentA, ComponentB>>() == 0);
}

CLASS_TEST_CASE("Components that are added to an entity are present in the include query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Include<ComponentA>>() == 2);
	CHECK(worldView.Count<ecs::query::Include<ComponentB>>() == 2);
	CHECK(worldView.Count<ecs::query::Include<ComponentA, ComponentB>>() == 1);

	// Query is cleared when component is removed.
	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityB);
	worldView.RemoveComponent<ComponentA>(raii.m_EntityC);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Include<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Include<ComponentB>>() == 0);
	CHECK(worldView.Count<ecs::query::Include<ComponentA, ComponentB>>() == 0);
}

CLASS_TEST_CASE("Components that are removed from an entity are present in the exclude query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	// #note: singleton component count towards being excluded
	CHECK(worldView.Count<ecs::query::Exclude<ComponentA>>() == 3);
	CHECK(worldView.Count<ecs::query::Exclude<ComponentB>>() == 3);
	CHECK(worldView.Count<ecs::query::Exclude<ComponentA, ComponentB>>() == 3);

	// Query is cleared when component is added.
	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Exclude<ComponentA>>() == 1);
	CHECK(worldView.Count<ecs::query::Exclude<ComponentB>>() == 1);
	CHECK(worldView.Count<ecs::query::Exclude<ComponentA, ComponentB>>() == 0);
}

CLASS_TEST_CASE("Components that are addedand its entity is destroyed in the same frame are only present in the removed query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.DestroyEntity(raii.m_EntityA);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Added<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 1);
}

CLASS_TEST_CASE("Components that are updatedand removed in the same frame are only present in the removed query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	entityWorld.Update({});

	worldView.WriteComponent<ComponentA>(raii.m_EntityA);
	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);

	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 1);
}

CLASS_TEST_CASE("Components that are updated the frame after a component was removed isn't present in either query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	entityWorld.Update({});

	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);
	entityWorld.Update({});

	worldView.WriteComponent<ComponentA>(raii.m_EntityA, false);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 0);
}

CLASS_TEST_CASE("Components that are updatedand its entity is destroyed in the same frame are only present in the removed query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	entityWorld.Update({});

	worldView.WriteComponent<ComponentA>(raii.m_EntityA);
	worldView.DestroyEntity(raii.m_EntityA);

	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 1);
}

CLASS_TEST_CASE("Components that are updated the frame after an entity is destroyed aren't present in any query.")
{
	using WorldView = ecs::WorldView
		::Write<ComponentA>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.WorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	entityWorld.Update({});

	worldView.DestroyEntity(raii.m_EntityA);
	entityWorld.Update({});

	worldView.WriteComponent<ComponentA>(raii.m_EntityA, false);
	entityWorld.Update({});

	CHECK(worldView.Count<ecs::query::Added<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Removed<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Updated<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Include<ComponentA>>() == 0);
	CHECK(worldView.Count<ecs::query::Exclude<ComponentA>>() == 2);
}
