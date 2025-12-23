#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "ECS/Component.h"
#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> { int m_Int = 1; };
	struct ComponentB final : public ecs::Component<ComponentB> { int m_Int = 2; };
	struct ComponentC final : public ecs::Component<ComponentC> { int m_Int = 3; };
	struct ComponentD final : public ecs::Component<ComponentD> { int m_Int = 4; };
}

TEST_CASE("ecs::EntityView. Test.")
{
	ecs::Entity entity;
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	entityWorld.RegisterComponent<ComponentC>();
	entityWorld.RegisterComponent<ComponentD>();
	entityWorld.Initialise();

	entity = entityWorld.CreateEntity();
	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.AddComponent<ComponentB>(entity);
	entityWorld.AddComponent<ComponentC>(entity);
	entityWorld.AddComponent<ComponentD>(entity);
	entityWorld.Update({});
	
	using World = ecs::WorldView
		::Write<ComponentA, ComponentB, ComponentC, ComponentD>;
	World world = entityWorld.WorldView<World>();

	using ViewA = ecs::EntityView
		::Required<ComponentA, ComponentB>
		::Optional<ComponentC, ComponentD>;
	ViewA viewA = world.EntityView<ViewA>(entity);
	{
		auto& writeCompA = viewA.WriteRequired<ComponentA>();
		const auto& readCompB = viewA.ReadRequired<ComponentB>();
		auto* writeCompC = viewA.WriteOptional<ComponentC>();
		const auto* readCompD = viewA.ReadOptional<ComponentD>();

		REQUIRE(writeCompC);
		REQUIRE(readCompD);
		CHECK(writeCompA.m_Int == 1);
		CHECK(readCompB.m_Int == 2);
		CHECK(writeCompC->m_Int == 3);
		CHECK(readCompD->m_Int == 4);
	}

	using ViewB = ecs::EntityView
		::Required<ComponentA>
		::Optional<ComponentC>;
	ViewB viewB(viewA);
	{
		auto& writeCompA = viewB.WriteRequired<ComponentA>();
		auto* writeCompC = viewB.WriteOptional<ComponentC>();

		REQUIRE(writeCompC);
		CHECK(writeCompA.m_Int == 1);
		CHECK(writeCompC->m_Int == 3);
	}

	{
		ecs::Entity copy = viewB;
		CHECK(copy == entity);
	}

	using Query = ecs::query
		::Include<ComponentA, const ComponentB>
		::Optional<ComponentC, const ComponentD>;
	for (auto&& view : world.Query<Query>())
	{
		auto& writeCompA = view.ReadRequired<ComponentA>();
		const auto& readCompB = view.ReadRequired<ComponentB>();
		auto* writeCompC = view.WriteOptional<ComponentC>();
		const auto* readCompD = view.ReadOptional<ComponentD>();

		REQUIRE(writeCompC);
		REQUIRE(readCompD);
		CHECK(writeCompA.m_Int == 1);
		CHECK(readCompB.m_Int == 2);
		CHECK(writeCompC->m_Int == 3);
		CHECK(readCompD->m_Int == 4);
	}
}