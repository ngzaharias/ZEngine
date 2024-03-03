#include <Catch2/catch.hpp>

#include <Core/TypeList.h>

#include <ECS/Component.h>
#include <ECS/EntityWorld.h>
#include <ECS/WorldView.h>
#include <ECS/QueryRegistry.h>

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
}

TEST_CASE("ecs::WorldView")
{
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	entityWorld.RegisterComponent<ComponentC>();
	entityWorld.RegisterComponent<SingletonA>();
	entityWorld.RegisterComponent<SingletonB>();
	entityWorld.RegisterComponent<SingletonC>();
	entityWorld.Initialise();

	using WorldView = ecs::WorldView<
		ComponentA, 
		const ComponentB, 
		const ComponentC,
		SingletonA, 
		const SingletonB, 
		const SingletonC,
		const ResourceA, const ResourceB>;
	WorldView worldView = entityWorld.GetWorldView<WorldView>();

	SECTION("CreateEntity")
	{
		CHECK(worldView.CreateEntity() == ecs::Entity(1));
		CHECK(worldView.CreateEntity() == ecs::Entity(2));
		CHECK(worldView.CreateEntity() == ecs::Entity(3));
		CHECK(worldView.CreateEntity() == ecs::Entity(4));
	}

	SECTION("DestroyEntity")
	{
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

	SECTION("IsAlive")
	{
		ecs::Entity entity;
		CHECK(!worldView.IsAlive(entity));
		entity = worldView.CreateEntity();
		entityWorld.Update({});

		CHECK(worldView.IsAlive(entity));
		worldView.DestroyEntity(entity);
		entityWorld.Update({});

		CHECK(!worldView.IsAlive(entity));
	}

	SECTION("HasComponent")
	{
		ecs::Entity entity = worldView.CreateEntity();

		CHECK(!worldView.HasComponent<ComponentA>(entity));
		CHECK(!worldView.HasComponent<ComponentB>(entity));

		worldView.AddComponent<ComponentA>(entity);
		worldView.AddComponent<ComponentB>(entity);
		entityWorld.Update({});

		CHECK(worldView.HasComponent<ComponentA>(entity));
		CHECK(worldView.HasComponent<ComponentB>(entity));
	}

	SECTION("HasSingleton")
	{
		entityWorld.Initialise();

		CHECK(!worldView.HasSingleton<SingletonA>());
		CHECK(!worldView.HasSingleton<SingletonB>());

		worldView.AddSingleton<SingletonA>();
		worldView.AddSingleton<SingletonB>();
		entityWorld.Update({});

		CHECK(worldView.HasSingleton<SingletonA>());
		CHECK(worldView.HasSingleton<SingletonB>());
	}

	SECTION("GetComponent")
	{
		//CHECK_THROWS(worldView.GetComponent<ComponentA>(entity));
		//CHECK_THROWS(worldView.GetComponent<ComponentB>(entity));

		ecs::Entity entity = worldView.CreateEntity();
		worldView.AddComponent<ComponentA>(entity);
		worldView.AddComponent<ComponentB>(entity);
		entityWorld.Update({});

		CHECK_NOTHROW(worldView.GetComponent<ComponentA>(entity));
		CHECK_NOTHROW(worldView.GetComponent<ComponentB>(entity));
	}

	SECTION("GetSingleton")
	{
		entityWorld.Initialise();

		//CHECK_THROWS(worldView.GetSingleton<SingletonA>());
		//CHECK_THROWS(worldView.GetSingleton<SingletonB>());

		worldView.AddSingleton<SingletonA>();
		worldView.AddSingleton<SingletonB>();
		entityWorld.Update({});

		CHECK_NOTHROW(worldView.GetSingleton<SingletonA>());
		CHECK_NOTHROW(worldView.GetSingleton<SingletonB>());
	}

	SECTION("AddComponent")
	{
		ecs::Entity entity = worldView.CreateEntity();

		SECTION("Single")
		{
			CHECK_NOTHROW(worldView.AddComponent<ComponentA>(entity));
			entityWorld.Update({});

			CHECK(worldView.HasComponent<ComponentA>(entity));
			CHECK(!worldView.HasComponent<ComponentB>(entity));
		}

		SECTION("Double")
		{
			worldView.AddComponent<ComponentA>(entity);
			entityWorld.Update({});
			CHECK(worldView.HasComponent<ComponentA>(entity));

			//entityWorld.AddComponent<ComponentA>(entity);
			//CHECK_THROWS(entityWorld.Update({}));
		}

		SECTION("Vardic")
		{
			const auto& componentA = worldView.AddComponent<ComponentA>(entity, true);
			const auto& componentB = worldView.AddComponent<ComponentB>(entity, -1337);
			CHECK(componentA.m_Bool == true);
			CHECK(componentB.m_Int32 == -1337);
		}
	}

	SECTION("AddSingleton")
	{
		entityWorld.Initialise();

		SECTION("Single")
		{
			CHECK_NOTHROW(worldView.AddSingleton<SingletonA>());
			entityWorld.Update({});

			CHECK(worldView.HasSingleton<SingletonA>());
			CHECK(!worldView.HasSingleton<SingletonB>());
		}

		SECTION("Double")
		{
			worldView.AddSingleton<SingletonA>();
			entityWorld.Update({});
			CHECK(worldView.HasSingleton<SingletonA>());

			//entityWorld.AddComponent<SingletonA>();
			//CHECK_THROWS(entityWorld.Update({}));
		}

		SECTION("Vardic")
		{
			const auto& componentA = worldView.AddSingleton<SingletonA>(true);
			const auto& componentB = worldView.AddSingleton<SingletonB>(-1337);
			CHECK(componentA.m_Bool == true);
			CHECK(componentB.m_Int32 == -1337);
		}
	}

	SECTION("RemoveComponent")
	{
		ecs::Entity entity = worldView.CreateEntity();
		worldView.AddComponent<ComponentA>(entity);
		worldView.AddComponent<ComponentB>(entity);
		entityWorld.Update({});

		SECTION("Single")
		{
			CHECK_NOTHROW(worldView.RemoveComponent<ComponentA>(entity));
			entityWorld.Update({});

			CHECK(!worldView.HasComponent<ComponentA>(entity));
			CHECK(worldView.HasComponent<ComponentB>(entity));
		}

		SECTION("Double")
		{
			CHECK_NOTHROW(worldView.RemoveComponent<ComponentA>(entity));
			entityWorld.Update({});

			CHECK(!worldView.HasComponent<ComponentA>(entity));
			CHECK(worldView.HasComponent<ComponentB>(entity));

			//worldView.RemoveComponent<ComponentA>(entity);
			//CHECK_THROWS(entityWorld.Update({}));
		}
	}

	SECTION("RemoveSingleton")
	{
		entityWorld.Initialise();

		worldView.AddSingleton<SingletonA>();
		worldView.AddSingleton<SingletonB>();
		entityWorld.Update({});

		SECTION("Single")
		{
			CHECK_NOTHROW(worldView.RemoveSingleton<SingletonA>());
			entityWorld.Update({});

			CHECK(!worldView.HasSingleton<SingletonA>());
			CHECK(worldView.HasSingleton<SingletonB>());
		}

		SECTION("Double")
		{
			CHECK_NOTHROW(worldView.RemoveSingleton<SingletonA>());
			entityWorld.Update({});

			CHECK(!worldView.HasSingleton<SingletonA>());
			CHECK(worldView.HasSingleton<SingletonB>());

			//worldView.RemoveSingleton<SingletonA>();
			//CHECK_THROWS(entityWorld.Update({}));
		}
	}

	SECTION("Query")
	{
		const ecs::Entity entityA = worldView.CreateEntity();
		const ecs::Entity entityB = worldView.CreateEntity();
		const ecs::Entity entityC = worldView.CreateEntity();
		const ecs::Entity entityAB = worldView.CreateEntity();

		worldView.AddComponent<ComponentA>(entityA);
		worldView.AddComponent<ComponentB>(entityB);
		worldView.AddComponent<ComponentC>(entityC);
		worldView.AddComponent<ComponentA>(entityAB);
		worldView.AddComponent<ComponentB>(entityAB);
		entityWorld.Update({});

		SECTION("Added")
		{
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

		SECTION("Removed")
		{
			worldView.RemoveComponent<ComponentA>(entityA);
			worldView.RemoveComponent<ComponentB>(entityB);
			worldView.RemoveComponent<ComponentC>(entityC);
			worldView.RemoveComponent<ComponentA>(entityAB);
			worldView.RemoveComponent<ComponentB>(entityAB);
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

		SECTION("Updated")
		{
			{
				INFO("Non-const calls to GetComponent are added to the Updated Query.");
				worldView.GetComponent<ComponentA>(entityA);
				worldView.GetComponent<ComponentB>(entityB);
				worldView.GetComponent<ComponentC>(entityC);
				worldView.GetComponent<ComponentA>(entityAB);
				worldView.GetComponent<ComponentB>(entityAB);

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
				worldView.GetComponent<const ComponentA>(entityA);
				worldView.GetComponent<const ComponentB>(entityB);
				worldView.GetComponent<const ComponentC>(entityC);
				worldView.GetComponent<const ComponentA>(entityAB);
				worldView.GetComponent<const ComponentB>(entityAB);

				entityWorld.Update({});
				CHECK(worldView.Query<ecs::query::Updated<ComponentA>>().IsEmpty());
				CHECK(worldView.Query<ecs::query::Updated<ComponentB>>().IsEmpty());
				CHECK(worldView.Query<ecs::query::Updated<ComponentC>>().IsEmpty());
				CHECK(worldView.Query<ecs::query::Updated<ComponentA, ComponentB>>().IsEmpty());
			}
		}

		SECTION("Include")
		{
			{
				CHECK(worldView.Query<ecs::query::Include<ComponentA>>().GetCount() == 2);
				CHECK(worldView.Query<ecs::query::Include<ComponentB>>().GetCount() == 2);
				CHECK(worldView.Query<ecs::query::Include<ComponentC>>().GetCount() == 1);
				CHECK(worldView.Query<ecs::query::Include<ComponentA, ComponentB>>().GetCount() == 1);
			}

			{
				INFO("Query is cleared when component is removed.");
				worldView.RemoveComponent<ComponentA>(entityA);
				worldView.RemoveComponent<ComponentB>(entityB);
				worldView.RemoveComponent<ComponentC>(entityC);
				worldView.RemoveComponent<ComponentA>(entityAB);
				worldView.RemoveComponent<ComponentB>(entityAB);
				entityWorld.Update({});

				CHECK(worldView.Query<ecs::query::Include<ComponentA>>().IsEmpty());
				CHECK(worldView.Query<ecs::query::Include<ComponentB>>().IsEmpty());
				CHECK(worldView.Query<ecs::query::Include<ComponentC>>().IsEmpty());
				CHECK(worldView.Query<ecs::query::Include<ComponentA, ComponentB>>().IsEmpty());
			}
		}

		SECTION("Exclude")
		{
			// #note: singleton component needs to be included
			CHECK(worldView.Query<ecs::query::Exclude<ComponentA>>().GetCount() == 3);
			CHECK(worldView.Query<ecs::query::Exclude<ComponentB>>().GetCount() == 3);
			CHECK(worldView.Query<ecs::query::Exclude<ComponentC>>().GetCount() == 4);
			CHECK(worldView.Query<ecs::query::Exclude<ComponentA, ComponentB>>().GetCount() == 2);

			{
				INFO("Query is cleared when component is added.");
				worldView.AddComponent<ComponentA>(entityB);
				worldView.AddComponent<ComponentA>(entityC);
				worldView.AddComponent<ComponentB>(entityA);
				worldView.AddComponent<ComponentB>(entityC);
				worldView.AddComponent<ComponentC>(entityA);
				worldView.AddComponent<ComponentC>(entityB);
				worldView.AddComponent<ComponentC>(entityAB);
				entityWorld.Update({});

				CHECK(worldView.Query<ecs::query::Exclude<ComponentA>>().GetCount() == 1);
				CHECK(worldView.Query<ecs::query::Exclude<ComponentB>>().GetCount() == 1);
				CHECK(worldView.Query<ecs::query::Exclude<ComponentC>>().GetCount() == 1);
				CHECK(worldView.Query<ecs::query::Exclude<ComponentA, ComponentB>>().GetCount() == 1);
			}
		}

		SECTION("Include + Exclude")
		{
			CHECK(worldView.Query<ecs::query::Include<ComponentA>::Exclude<ComponentB>>().GetCount() == 1);
			CHECK(worldView.Query<ecs::query::Include<ComponentB>::Exclude<ComponentA>>().GetCount() == 1);
		}
	}
}