#include "SharedCursor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void shared::cursor::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<shared::cursor::TransformComponent>();
	}

	// events
	{
		world.RegisterEvent<shared::cursor::ClientTransformEvent>();
	}
}