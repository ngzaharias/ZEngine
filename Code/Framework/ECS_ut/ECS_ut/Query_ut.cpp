#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "Core/Types.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryRange.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::Query. " name, "[ecs::Query]")

namespace
{
	struct ComponentA final : public ecs::Component { bool m_Bool = true; };
	struct ComponentB final : public ecs::Component { bool m_Bool = false; };
	struct ComponentC final : public ecs::Component { };

	using World = ecs::WorldView
		::Write<
		ComponentA,
		ComponentB,
		ComponentC>;

	struct WorldWrapper
	{
		WorldWrapper()
			: m_TypeRegistry()
			, m_EntityWorld(m_TypeRegistry)
		{
			m_EntityWorld.RegisterComponent<ComponentA>();
			m_EntityWorld.RegisterComponent<ComponentB>();
			m_EntityWorld.RegisterComponent<ComponentC>();
			m_EntityWorld.Initialise();
		};

		void Update()
		{
			m_EntityWorld.Update({});
		};

		World GetWorld()
		{
			return m_EntityWorld.WorldView<World>();
		};

		ecs::EntityWorld m_EntityWorld;
		ecs::TypeRegistry m_TypeRegistry;
	};
}

#pragma region ADDED

CLASS_TEST_CASE("Added query doesn't trigger when an entity is created without the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when an entity is created without the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query does trigger when an entity is created with the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Added query doesn't trigger when an entity is created with the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query does trigger when a component is added (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is added (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is updated (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is updated (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is added and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query does trigger when a component is added and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is updated and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is updated and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is removed and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Added query doesn't trigger when a component is removed and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Added<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

#pragma endregion

#pragma region REMOVED

CLASS_TEST_CASE("Removed query doesn't trigger when an entity is created without the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when an entity is created without the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when an entity is created with the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when an entity is created with the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is added (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is added (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is updated (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is updated (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query does trigger when a component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query does trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is added and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query does trigger when a component is added and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is updated and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query does trigger when a component is updated and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed query doesn't trigger when a component is removed and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed query does trigger when a component is removed and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed + Include query does trigger when one component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>
		::Include<ComponentB>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed + Include query doesn't trigger when both components are removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>
		::Include<ComponentB>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.RemoveComponent<ComponentB>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed + Include query doesn't trigger when both components are removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>
		::Include<ComponentB>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.RemoveComponent<ComponentB>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed + Include query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Removed<ComponentA>
		::Include<ComponentB>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Removed + Include query does trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Removed<ComponentA>
		::Include<ComponentB>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Removed + Include query does trigger when an entity is destroyed (alive + dead).")
{
	using Query = ecs::query
		::Condition<ecs::Alive, ecs::Dead>
		::Removed<ComponentA>
		::Include<ComponentB>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

#pragma endregion

#pragma region UPDATED

CLASS_TEST_CASE("Updated query doesn't trigger when an entity is created without the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when an entity is created without the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when an entity is created with the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when an entity is created with the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is added (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is added (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query does trigger when a component is updated (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is updated (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is added and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is added and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is updated and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query does trigger when a component is updated and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is removed and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Updated query doesn't trigger when a component is removed and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Updated<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

#pragma endregion

#pragma region INCLUDE

CLASS_TEST_CASE("Include query doesn't trigger when an entity is created without the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query doesn't trigger when an entity is created without the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when an entity is created with the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Include query doesn't trigger when an entity is created with the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when a component is added (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is added (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when a component is updated (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is updated (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is added and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when a component is added and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is updated and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when a component is updated and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Include query doesn't trigger when a component is removed and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Include query does trigger when a component is removed and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Include<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

#pragma endregion

#pragma region OPTIONAL

CLASS_TEST_CASE("Optional query does trigger when an entity is created without the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	// results in 2 queries, 1 for the test and 1 for the static table
	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Optional query doesn't trigger when an entity is created without the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Optional query does trigger when an entity is created with the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	// results in 2 queries, 1 for the test and 1 for the static table
	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Optional query doesn't trigger when an entity is created with the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Optional query does trigger when a component is added (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Optional query doesn't trigger when a component is added (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Optional query does trigger when a component is updated (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	// results in 2 queries, 1 for the test and 1 for the static table
	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Optional query doesn't trigger when a component is updated (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Optional query does trigger when a component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	// results in 2 queries, 1 for the test and 1 for the static table
	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Optional query doesn't trigger when a component is removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Optional query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query does trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query doesn't trigger when a component is added and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query does trigger when a component is added and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query doesn't trigger when a component is updated and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query does trigger when a component is updated and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query doesn't trigger when a component is removed and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Optional query does trigger when a component is removed and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Optional<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 1);
}

#pragma endregion

#pragma region EXCLUDE

CLASS_TEST_CASE("Exclude query does trigger when an entity is created without the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	// results in 2 queries, 1 for the test and 1 for the static table
	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when an entity is created without the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when an entity is created with the component (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when an entity is created with the component (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is added (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is added (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is updated (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is updated (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query does trigger when a component is removed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	// results in 2 queries, 1 for the test and 1 for the static table
	CHECK(world.Count<Query>() == 2);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is removed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when an entity is destroyed (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when an entity is destroyed (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is added and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is added and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	wrapper.Update();

	world.AddComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is updated and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is updated and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.WriteComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is removed and the entity is destroyed in the same frame (alive).")
{
	using Query = ecs::query
		::Condition<ecs::Alive>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	// 1 for the static table
	CHECK(world.Count<Query>() == 1);
}

CLASS_TEST_CASE("Exclude query doesn't trigger when a component is removed and the entity is destroyed in the same frame (dead).")
{
	using Query = ecs::query
		::Condition<ecs::Dead>
		::Exclude<ComponentA>;

	WorldWrapper wrapper;
	World world = wrapper.GetWorld();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	wrapper.Update();

	world.RemoveComponent<ComponentA>(entity);
	world.DestroyEntity(entity);
	wrapper.Update();

	CHECK(world.Count<Query>() == 0);
}

#pragma endregion
