#include "ClientCursorPCH.h"
#include "ClientCursor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ClientCursor/CursorTransformSystem.h"

void client::cursor::RegisterModule(ecs::EntityWorld& world)
{
	// system
	{
		world.RegisterSystem<client::cursor::TransformSystem>();
	}
}