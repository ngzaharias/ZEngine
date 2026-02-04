#include "ClientCursorPCH.h"
#include "ClientCursor/CursorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ClientCursor/CursorTransformSystem.h"

void client::cursor::RegisterModule(ecs::EntityWorld& world)
{
	// system
	{
		world.RegisterSystem<client::cursor::TransformSystem>();
	}
}