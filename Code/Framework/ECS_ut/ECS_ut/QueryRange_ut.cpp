#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "Core/Types.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryRange.h"
#include "ECS/QueryTypes.h"

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> { bool m_Bool = true; };
	struct ComponentB final : public ecs::Component<ComponentB> { bool m_Bool = false; };
	struct ComponentC final : public ecs::Component<ComponentC> { };
}

TEST_CASE("ecs::QueryRange. Test.")
{
	ecs::Entity entity;
	ecs::EntityWorld world;
	world.RegisterComponent<ComponentA>();
	world.RegisterComponent<ComponentB>();
	world.RegisterComponent<ComponentC>();
	world.Initialise();

	entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.AddComponent<ComponentB>(entity);
	world.Update({});

	Set<ecs::Entity> entities;
	entities.Add(entity);

	using Query = ecs::query
		::Include<ComponentA, const ComponentB>
		::Optional<ComponentB, const ComponentC>;
	
	ecs::QueryRange<Query> range{ world, entities };

	auto itr = range.begin();
	auto view = *itr;
	
	auto& componentA = view.ReadRequired<ComponentA>();
	CHECK(componentA.m_Bool);
	
	auto* componentB = view.ReadOptional<ComponentB>();
	REQUIRE(componentB);
	CHECK(!componentB->m_Bool);

	auto* componentC = view.ReadOptional<const ComponentC>();
	CHECK(!componentC);
}