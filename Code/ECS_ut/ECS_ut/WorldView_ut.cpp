#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ECS/QueryRegistry.h"

namespace
{
	struct ComponentA : public ecs::Component<ComponentA> {};
	struct ComponentB : public ecs::Component<ComponentB> {};

	struct SingletonA : public ecs::SingletonComponent<SingletonA> {};
	struct SingletonB : public ecs::SingletonComponent<SingletonB> {};

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

TEST_CASE("ecs::WorldView. CreateEntity.")
{
	using WorldView = ecs::WorldView<>;

	ecs::EntityWorld entityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK(worldView.CreateEntity() == ecs::Entity(1));
	CHECK(worldView.CreateEntity() == ecs::Entity(2));
	CHECK(worldView.CreateEntity() == ecs::Entity(3));
	CHECK(worldView.CreateEntity() == ecs::Entity(4));
	CHECK(worldView.CreateEntity() == ecs::Entity(5));
}

TEST_CASE("ecs::WorldView. DestroyEntity.")
{
	using WorldView = ecs::WorldView<>;

	ecs::EntityWorld entityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	ecs::Entity entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 0);

	worldView.DestroyEntity(entity);
	entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 2);
	CHECK(entity.GetVersion() == 0);

	worldView.DestroyEntity(entity);
	entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(entity.GetIndex() == 3);
	CHECK(entity.GetVersion() == 0);

	entity = worldView.CreateEntity();
	CHECK(entity.GetIndex() == 1);
	CHECK(entity.GetVersion() == 1);
}

TEST_CASE("ecs::WorldView. IsAlive.")
{
	using WorldView = ecs::WorldView<>;

	ecs::EntityWorld entityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	ecs::Entity entity;
	CHECK(!worldView.IsAlive(entity));
	entity = worldView.CreateEntity();
	entityWorld.Update({});

	CHECK(worldView.IsAlive(entity));
	worldView.DestroyEntity(entity);
	entityWorld.Update({});

	CHECK(!worldView.IsAlive(entity));
}

TEST_CASE("ecs::WorldView. AddComponent.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();
	ecs::Entity entity = worldView.CreateEntity();

	CHECK_NOTHROW(worldView.AddComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(worldView.HasComponent<ComponentA>(entity));
	CHECK(!worldView.HasComponent<ComponentB>(entity));
}

TEST_CASE("ecs::WorldView. RemoveComponent.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	ecs::Entity entity = worldView.CreateEntity();
	worldView.AddComponent<ComponentA>(entity);
	worldView.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.RemoveComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(!worldView.HasComponent<ComponentA>(entity));
	CHECK(worldView.HasComponent<ComponentB>(entity));
}

TEST_CASE("ecs::WorldView. HasComponent.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK(!worldView.HasComponent<ComponentA>(raii.m_EntityA));
	CHECK(!worldView.HasComponent<ComponentB>(raii.m_EntityA));

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityA);
	entityWorld.Update({});

	CHECK(worldView.HasComponent<ComponentA>(raii.m_EntityA));
	CHECK(worldView.HasComponent<ComponentB>(raii.m_EntityA));
}

TEST_CASE("ecs::WorldView. ReadComponent.")
{
	using WorldView = ecs::WorldView<const ComponentA, const ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityA);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.ReadComponent<ComponentA>(raii.m_EntityA));
	CHECK_NOTHROW(worldView.ReadComponent<ComponentB>(raii.m_EntityA));
}

TEST_CASE("ecs::WorldView. WriteComponent.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityA);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.WriteComponent<ComponentA>(raii.m_EntityA));
	CHECK_NOTHROW(worldView.WriteComponent<ComponentB>(raii.m_EntityA));
}

TEST_CASE("ecs::WorldView. ReadSingleton.")
{
	using WorldView = ecs::WorldView<const SingletonA, const SingletonB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK_NOTHROW(worldView.ReadSingleton<SingletonA>());
	CHECK_NOTHROW(worldView.ReadSingleton<SingletonB>());
}

TEST_CASE("ecs::WorldView. WriteSingleton.")
{
	using WorldView = ecs::WorldView<SingletonA, SingletonB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK_NOTHROW(worldView.WriteSingleton<SingletonA>());
	CHECK_NOTHROW(worldView.WriteSingleton<SingletonB>());
}

TEST_CASE("ecs::WorldView. ReadResource.")
{
	using WorldView = ecs::WorldView<const ResourceA, const ResourceB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK_NOTHROW(worldView.ReadResource<ResourceA>());
	CHECK_NOTHROW(worldView.ReadResource<ResourceB>());
}

TEST_CASE("ecs::WorldView. WriteResource.")
{
	using WorldView = ecs::WorldView<ResourceA, ResourceB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK_NOTHROW(worldView.WriteResource<ResourceA>());
	CHECK_NOTHROW(worldView.WriteResource<ResourceB>());
}

TEST_CASE("ecs::WorldView. Components that are added to an entity are present in the added query.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	// Query is active on the next frame.
	CHECK(worldView.Query<ecs::query::Added<ComponentA>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Added<ComponentB>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Added<ComponentA, ComponentB>>().GetCount() == 1);

	// Query is cleared on the frame after.
	entityWorld.Update({});

	CHECK(worldView.Query<ecs::query::Added<ComponentA>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Added<ComponentB>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Added<ComponentA, ComponentB>>().IsEmpty());
}

TEST_CASE("ecs::WorldView. Components that are removed from an entity are present in the removed query.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

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
	CHECK(worldView.Query<ecs::query::Removed<ComponentA>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Removed<ComponentB>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Removed<ComponentA, ComponentB>>().GetCount() == 1);

	entityWorld.Update({});

	// Query is cleared on the frame after.
	CHECK(worldView.Query<ecs::query::Removed<ComponentA>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Removed<ComponentB>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Removed<ComponentA, ComponentB>>().IsEmpty());
}

TEST_CASE("ecs::WorldView. Updated Query isn't triggered when using ReadComponent.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

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

	CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().IsEmpty());
}

TEST_CASE("ecs::WorldView. Updated Query isn't triggered when using WriteComponent.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

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
	CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().GetCount() == 1);

	entityWorld.Update({});

	// Query is cleared on the frame after.
	CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().IsEmpty());
}

TEST_CASE("ecs::WorldView. Components that are added to an entity are present in the include query.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Query<ecs::query::Include<ComponentA>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Include<ComponentB>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Include<ComponentA, ComponentB>>().GetCount() == 1);

	// Query is cleared when component is removed.
	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityB);
	worldView.RemoveComponent<ComponentA>(raii.m_EntityC);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Query<ecs::query::Include<ComponentA>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Include<ComponentB>>().IsEmpty());
	CHECK(worldView.Query<ecs::query::Include<ComponentA, ComponentB>>().IsEmpty());
}

TEST_CASE("ecs::WorldView. Components that are removed from an entity are present in the exclude query.")
{
	using WorldView = ecs::WorldView<ComponentA, ComponentB>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	// #note: singleton component count towards being excluded
	CHECK(worldView.Query<ecs::query::Exclude<ComponentA>>().GetCount() == 4);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentB>>().GetCount() == 4);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentA, ComponentB>>().GetCount() == 4);

	// Query is cleared when component is added.
	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	worldView.AddComponent<ComponentB>(raii.m_EntityB);
	worldView.AddComponent<ComponentA>(raii.m_EntityC);
	worldView.AddComponent<ComponentB>(raii.m_EntityC);
	entityWorld.Update({});

	CHECK(worldView.Query<ecs::query::Exclude<ComponentA>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentB>>().GetCount() == 2);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentA, ComponentB>>().GetCount() == 1);
}

TEST_CASE("ecs::WorldView. Components that are updated and removed in the same frame are only present in the removed query.")
{
	using WorldView = ecs::WorldView<ComponentA>;

	RAIIHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	worldView.AddComponent<ComponentA>(raii.m_EntityA);
	entityWorld.Update({});

	worldView.WriteComponent<ComponentA>(raii.m_EntityA);
	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);

	entityWorld.Update({});

	CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().GetCount() == 0);
	CHECK(worldView.Query<ecs::query::Removed<ComponentA>>().GetCount() == 1);
}
