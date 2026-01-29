#include "ServerCursorPCH.h"
#include "ServerCursor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/TypeRegistry.h"
#include "ServerCursor/CursorTransformSingleton.h"
#include "ServerCursor/CursorTransformSystem.h"

void server::cursor::RegisterModule(ecs::EntityWorld& world)
{
	// singleton
	{
		world.RegisterSingleton<server::cursor::TransformSingleton>();
	}

	// system
	{
		world.RegisterSystem<server::cursor::TransformSystem>();
	}
}