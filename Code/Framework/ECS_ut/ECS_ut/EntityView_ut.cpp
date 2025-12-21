#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "ECS/Component.h"
#include "ECS/EntityView.h"
#include "ECS/EntityWorld.h"

namespace
{
	struct ComponentA final : public ecs::Component<ComponentA> {};
	struct ComponentB final : public ecs::Component<ComponentB> {};
}

TEST_CASE("ecs::EntityView. Test.")
{
	ecs::Entity entity;
	ecs::EntityWorld world;
	world.RegisterComponent<ComponentA>();
	world.RegisterComponent<ComponentB>();
	world.Initialise();

	entity = world.CreateEntity();
	world.AddComponent<ComponentA>(entity);
	world.Update({});

	using View = ecs::EntityView
		::Required<ComponentA>
		::Optional<ComponentB>;
	View view = world.EntityView<View>(entity);

	const auto& readCompA = view.ReadRequired<ComponentA>();
	const auto* readCompB = view.ReadOptional<ComponentB>();
	auto& writeCompA = view.WriteRequired<ComponentA>();
	auto* writeCompB = view.WriteOptional<ComponentB>();
}