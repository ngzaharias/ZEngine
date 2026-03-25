#include "ServerCursorPCH.h"
#include "ServerCursor/CursorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/TypeRegistry.h"
#include "ServerCursor/CursorTransformComponent.h"
#include "ServerCursor/CursorTransformSystem.h"

void server::cursor::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<server::cursor::TransformComponent>();
	world.RegisterSystem<server::cursor::TransformSystem>();
}