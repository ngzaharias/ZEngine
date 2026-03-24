#include "ServerCursorPCH.h"
#include "ServerCursor/CursorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/TypeRegistry.h"
#include "ServerCursor/CursorTransformStaticComponent.h"
#include "ServerCursor/CursorTransformSystem.h"

void server::cursor::RegisterModule(ecs::EntityWorld& world)
{
	// component
	{
		world.RegisterComponent<server::cursor::TransformStaticComponent>();
	}

	// system
	{
		world.RegisterSystem<server::cursor::TransformSystem>();
	}
}