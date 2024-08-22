#include <Catch2/catch.hpp>

#include "Core/TypeList.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ECS/QueryRegistry.h"

class GameTime {};

namespace
{
	struct ComponentA : public ecs::Component<ComponentA> 
	{ 
		ComponentA() = default;
		ComponentA(bool val) : m_Bool(val) { }
		bool m_Bool = false; 
	};

	struct ComponentB : public ecs::Component<ComponentB> 
	{ 
		ComponentB() = default;
		ComponentB(int32 val) : m_Int32(val) { }
		int32 m_Int32 = 0;
	};

	struct ComponentC : public ecs::Component<ComponentC>
	{
		ComponentC() = default;
		ComponentC(float val) : m_Float(val) { }
		float m_Float = 0.f;
	};

	struct SingletonA : public ecs::SingletonComponent<SingletonA>
	{
		SingletonA() = default;
		SingletonA(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};

	struct SingletonB : public ecs::SingletonComponent<SingletonB>
	{
		SingletonB() = default;
		SingletonB(int32 val) : m_Int32(val) { }
		int32 m_Int32 = 0;
	};

	struct SingletonC : public ecs::SingletonComponent<SingletonC>
	{
		SingletonC() = default;
		SingletonC(float val) : m_Float(val) { }
		float m_Float = 0.f;
	};

	class ResourceA { };
	class ResourceB { };

	using WorldView = ecs::WorldView<
		ComponentA,
		const ComponentB,
		const ComponentC,
		SingletonA,
		const SingletonB,
		const SingletonC,
		const ResourceA, 
		const ResourceB>;

	struct RAIIHelper
	{
		RAIIHelper()
		{
			m_EntityWorld.RegisterComponent<ComponentA>();
			m_EntityWorld.RegisterComponent<ComponentB>();
			m_EntityWorld.RegisterComponent<ComponentC>();
			m_EntityWorld.RegisterSingleton<SingletonA>();
			m_EntityWorld.RegisterSingleton<SingletonB>();
			m_EntityWorld.RegisterSingleton<SingletonC>();
		}

		ecs::EntityWorld m_EntityWorld;
	};

	struct QueryHelper
	{
		QueryHelper()
		{
			m_EntityWorld.RegisterComponent<ComponentA>();
			m_EntityWorld.RegisterComponent<ComponentB>();
			m_EntityWorld.RegisterComponent<ComponentC>();
			m_EntityWorld.RegisterSingleton<SingletonA>();
			m_EntityWorld.RegisterSingleton<SingletonB>();
			m_EntityWorld.RegisterSingleton<SingletonC>();
			m_EntityWorld.Initialise();

			WorldView worldView = m_EntityWorld.GetWorldView<WorldView>();

			m_EntityA = worldView.CreateEntity();
			m_EntityB = worldView.CreateEntity();
			m_EntityC = worldView.CreateEntity();
			m_EntityAB = worldView.CreateEntity();

			worldView.AddComponent<ComponentA>(m_EntityA);
			worldView.AddComponent<ComponentB>(m_EntityB);
			worldView.AddComponent<ComponentC>(m_EntityC);
			worldView.AddComponent<ComponentA>(m_EntityAB);
			worldView.AddComponent<ComponentB>(m_EntityAB);
			m_EntityWorld.Update({});
		}

		ecs::EntityWorld m_EntityWorld = {};
		ecs::Entity m_EntityA = {};
		ecs::Entity m_EntityB = {};
		ecs::Entity m_EntityC = {};
		ecs::Entity m_EntityAB = {};
	};
}

TEST_CASE("ecs::WorldView. CreateEntity.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK(worldView.CreateEntity() == ecs::Entity(0));
	CHECK(worldView.CreateEntity() == ecs::Entity(1));
	CHECK(worldView.CreateEntity() == ecs::Entity(2));
	CHECK(worldView.CreateEntity() == ecs::Entity(3));
}

TEST_CASE("ecs::WorldView. DestroyEntity.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

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

TEST_CASE("ecs::WorldView. IsAlive.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
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

TEST_CASE("ecs::WorldView. HasComponent.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	ecs::Entity entity = worldView.CreateEntity();
	CHECK(!worldView.HasComponent<ComponentA>(entity));
	CHECK(!worldView.HasComponent<ComponentB>(entity));

	worldView.AddComponent<ComponentA>(entity);
	worldView.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK(worldView.HasComponent<ComponentA>(entity));
	CHECK(worldView.HasComponent<ComponentB>(entity));
}

TEST_CASE("ecs::WorldView. GetComponent.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	//CHECK_THROWS(worldView.WriteComponent<ComponentA>(entity));
	//CHECK_THROWS(worldView.WriteComponent<ComponentB>(entity));

	ecs::Entity entity = worldView.CreateEntity();
	worldView.AddComponent<ComponentA>(entity);
	worldView.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.WriteComponent<ComponentA>(entity));
	CHECK_NOTHROW(worldView.WriteComponent<ComponentB>(entity));
}

TEST_CASE("ecs::WorldView. GetSingleton.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	entityWorld.Initialise();

	//CHECK_THROWS(worldView.WriteSingleton<SingletonA>());
	//CHECK_THROWS(worldView.WriteSingleton<SingletonB>());

	entityWorld.Update({});

	WorldView worldView = entityWorld.GetWorldView<WorldView>();
	CHECK_NOTHROW(worldView.WriteSingleton<SingletonA>());
	CHECK_NOTHROW(worldView.WriteSingleton<SingletonB>());
}

TEST_CASE("ecs::WorldView. AddComponent - Single.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();
	ecs::Entity entity = worldView.CreateEntity();

	CHECK_NOTHROW(worldView.AddComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(worldView.HasComponent<ComponentA>(entity));
	CHECK(!worldView.HasComponent<ComponentB>(entity));
}

TEST_CASE("ecs::WorldView. AddComponent - Double.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();
	ecs::Entity entity = worldView.CreateEntity();

	worldView.AddComponent<ComponentA>(entity);
	entityWorld.Update({});
	CHECK(worldView.HasComponent<ComponentA>(entity));

	//entityWorld.AddComponent<ComponentA>(entity);
	//CHECK_THROWS(entityWorld.Update({}));
}

TEST_CASE("ecs::WorldView. AddComponent - Vardic.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();
	ecs::Entity entity = worldView.CreateEntity();

	const auto& componentA = worldView.AddComponent<ComponentA>(entity, true);
	const auto& componentB = worldView.AddComponent<ComponentB>(entity, -1337);
	CHECK(componentA.m_Bool == true);
	CHECK(componentB.m_Int32 == -1337);
}

TEST_CASE("ecs::WorldView. RemoveComponent - Single.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
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

TEST_CASE("ecs::WorldView. RemoveComponent - Double.")
{
	RAIIHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	ecs::Entity entity = worldView.CreateEntity();
	worldView.AddComponent<ComponentA>(entity);
	worldView.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	CHECK_NOTHROW(worldView.RemoveComponent<ComponentA>(entity));
	entityWorld.Update({});

	CHECK(!worldView.HasComponent<ComponentA>(entity));
	CHECK(worldView.HasComponent<ComponentB>(entity));

	//worldView.RemoveComponent<ComponentA>(entity);
	//CHECK_THROWS(entityWorld.Update({}));
}

TEST_CASE("ecs::WorldView. Query - Added.")
{
	QueryHelper raiihelper;
	ecs::EntityWorld& entityWorld = raiihelper.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	{
		INFO("Query is active on the next frame.");
		CHECK(worldView.Query<ecs::query::Added<ComponentA>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Added<ComponentB>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Added<ComponentC>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Added<ComponentA, ComponentB>>().GetCount() == 1);
	}

	{
		INFO("Query is cleared on the frame after.");
		entityWorld.Update({});

		CHECK(worldView.Query<ecs::query::Added<ComponentA>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Added<ComponentB>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Added<ComponentC>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Added<ComponentA, ComponentB>>().IsEmpty());
	}
}

TEST_CASE("ecs::WorldView. Query - Removed.")
{
	QueryHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	worldView.RemoveComponent<ComponentA>(raii.m_EntityA);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityB);
	worldView.RemoveComponent<ComponentC>(raii.m_EntityC);
	worldView.RemoveComponent<ComponentA>(raii.m_EntityAB);
	worldView.RemoveComponent<ComponentB>(raii.m_EntityAB);
	entityWorld.Update({});

	{
		INFO("Query is active on the next frame.");
		CHECK(worldView.Query<ecs::query::Removed<ComponentA>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Removed<ComponentB>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Removed<ComponentC>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Removed<ComponentA, ComponentB>>().GetCount() == 1);
	}

	entityWorld.Update({});

	{
		INFO("Query is cleared on the frame after.");
		CHECK(worldView.Query<ecs::query::Removed<ComponentA>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Removed<ComponentB>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Removed<ComponentC>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Removed<ComponentA, ComponentB>>().IsEmpty());
	}
}

TEST_CASE("ecs::WorldView. Query - Updated.")
{
	QueryHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	{
		INFO("Non-const calls to GetComponent are added to the Updated Query.");
		worldView.WriteComponent<ComponentA>(raii.m_EntityA);
		worldView.WriteComponent<ComponentB>(raii.m_EntityB);
		worldView.WriteComponent<ComponentC>(raii.m_EntityC);
		worldView.WriteComponent<ComponentA>(raii.m_EntityAB);
		worldView.WriteComponent<ComponentB>(raii.m_EntityAB);

		INFO("Query is active on the next frame.");
		entityWorld.Update({});
		CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Updated<ComponentC>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().GetCount() == 1);

		INFO("Query is cleared on the frame after.");
		entityWorld.Update({});
		CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Updated<ComponentC>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().IsEmpty());
	}

	{
		INFO("Const calls to GetComponent are not added to the Updated Query.");
		worldView.ReadComponent< ComponentA>(raii.m_EntityA);
		worldView.ReadComponent< ComponentB>(raii.m_EntityB);
		worldView.ReadComponent< ComponentC>(raii.m_EntityC);
		worldView.ReadComponent< ComponentA>(raii.m_EntityAB);
		worldView.ReadComponent< ComponentB>(raii.m_EntityAB);

		entityWorld.Update({});
		CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Updated<ComponentC>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().IsEmpty());
	}
}

TEST_CASE("ecs::WorldView. Query - Include.")
{
	QueryHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	{
		CHECK(worldView.Query<ecs::query::Include<ComponentA>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Include<ComponentB>>().GetCount() == 2);
		CHECK(worldView.Query<ecs::query::Include<ComponentC>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Include<ComponentA, ComponentB>>().GetCount() == 1);
	}

	{
		INFO("Query is cleared when component is removed.");
		worldView.RemoveComponent<ComponentA>(raii.m_EntityA);
		worldView.RemoveComponent<ComponentB>(raii.m_EntityB);
		worldView.RemoveComponent<ComponentC>(raii.m_EntityC);
		worldView.RemoveComponent<ComponentA>(raii.m_EntityAB);
		worldView.RemoveComponent<ComponentB>(raii.m_EntityAB);
		entityWorld.Update({});

		CHECK(worldView.Query<ecs::query::Include<ComponentA>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Include<ComponentB>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Include<ComponentC>>().IsEmpty());
		CHECK(worldView.Query<ecs::query::Include<ComponentA, ComponentB>>().IsEmpty());
	}
}

TEST_CASE("ecs::WorldView. Query - Exclude.")
{
	QueryHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	// #note: singleton component needs to be included
	CHECK(worldView.Query<ecs::query::Exclude<ComponentA>>().GetCount() == 3);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentB>>().GetCount() == 3);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentC>>().GetCount() == 4);
	CHECK(worldView.Query<ecs::query::Exclude<ComponentA, ComponentB>>().GetCount() == 2);

	{
		INFO("Query is cleared when component is added.");
		worldView.AddComponent<ComponentA>(raii.m_EntityB);
		worldView.AddComponent<ComponentA>(raii.m_EntityC);
		worldView.AddComponent<ComponentB>(raii.m_EntityA);
		worldView.AddComponent<ComponentB>(raii.m_EntityC);
		worldView.AddComponent<ComponentC>(raii.m_EntityA);
		worldView.AddComponent<ComponentC>(raii.m_EntityB);
		worldView.AddComponent<ComponentC>(raii.m_EntityAB);
		entityWorld.Update({});

		CHECK(worldView.Query<ecs::query::Exclude<ComponentA>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Exclude<ComponentB>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Exclude<ComponentC>>().GetCount() == 1);
		CHECK(worldView.Query<ecs::query::Exclude<ComponentA, ComponentB>>().GetCount() == 1);
	}
}

TEST_CASE("ecs::WorldView. Query - Include + Exclude.")
{
	QueryHelper raii;
	ecs::EntityWorld& entityWorld = raii.m_EntityWorld;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	CHECK(worldView.Query<ecs::query::Include<ComponentA>::Exclude<ComponentB>>().GetCount() == 1);
	CHECK(worldView.Query<ecs::query::Include<ComponentB>::Exclude<ComponentA>>().GetCount() == 1);
}
