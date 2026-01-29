#include "ClientCursorPCH.h"
#include "ClientCursor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ClientCursor/CursorRenderComponent.h"
#include "ClientCursor/CursorRenderSystem.h"
#include "ClientCursor/CursorTransformSystem.h"

void client::cursor::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<client::cursor::RenderComponent>();
	}

	// system
	{
		world.RegisterSystem<client::cursor::RenderSystem>();
		world.RegisterSystem<client::cursor::TransformSystem>();
	}
}