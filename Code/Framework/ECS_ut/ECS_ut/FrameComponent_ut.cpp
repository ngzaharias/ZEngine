#include <Catch2/catch.hpp>

#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "ECS/Event.h"
#include "ECS/Singleton.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ecs::FrameComponent. " name, "[ecs::FrameComponent]")

class GameTime { };

namespace
{
	struct Component final : public ecs::FrameComponent<Component>
	{
		Component() = default;
		Component(bool val) : m_Bool(val) { }
		bool m_Bool = false;
	};
}

CLASS_TEST_CASE("AddComponent can add a frame component to an entity.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	CHECK(world.HasComponent<Component>(entity));
}

CLASS_TEST_CASE("Component is removed at the end of the next frame.")
{
	ecs::TypeRegistry types;
	ecs::EntityWorld world(types);
	world.RegisterComponent<Component>();

	ecs::Entity entity = world.CreateEntity();
	world.AddComponent<Component>(entity);
	world.Update({});
	world.Update({});
	CHECK(!world.HasComponent<Component>(entity));
}
