#include "SharedCursor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/TypeRegistry.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void shared::cursor::RegisterModule(ecs::EntityWorld& world)
{
	auto& registry = world.WriteResource<ecs::TypeRegistry>();

	// components
	{
		world.RegisterComponent<shared::cursor::TransformComponent>();
		registry.RegisterComponent<shared::cursor::TransformComponent>();
	}

	// events
	{
		world.RegisterEvent<shared::cursor::ClientTransformEvent>();
		registry.RegisterEvent<shared::cursor::ClientTransformEvent>();
	}
}