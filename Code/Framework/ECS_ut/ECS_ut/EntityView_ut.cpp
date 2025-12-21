#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "ECS/Component.h"
#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> {};
	struct ComponentB final : public ecs::Component<ComponentB> {};
}

TEST_CASE("ecs::EntityView. Test.")
{
	ecs::Entity entity;
	ecs::EntityWorld entityWorld;
	entityWorld.RegisterComponent<ComponentA>();
	entityWorld.RegisterComponent<ComponentB>();
	entityWorld.Initialise();

	entity = entityWorld.CreateEntity();
	entityWorld.AddComponent<ComponentA>(entity);
	entityWorld.AddComponent<ComponentB>(entity);
	entityWorld.Update({});

	using View = ecs::EntityView
		::Required<ComponentA>
		::Optional<ComponentB>;
	View view = entityWorld.EntityView<View>(entity);

	const auto& readCompA = view.ReadRequired<ComponentA>();
	const auto* readCompB = view.ReadOptional<ComponentB>();
	auto& writeCompA = view.WriteRequired<ComponentA>();
	auto* writeCompB = view.WriteOptional<ComponentB>();

	using World = ecs::WorldView<
		ComponentA,
		ComponentB>;

	using Query = ecs::query::Include<
		ComponentA,
		ComponentB>;

	World world = entityWorld.WorldView<World>();
	for (auto&& view : world.Query<Query>())
	{
		auto& componentA = view.ReadRequired<ComponentA>();
		auto& componentB = view.ReadRequired<ComponentB>();

		ecs::Entity entity = view;
	}
}