#include "SharedCursor/CursorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

void shared::cursor::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::cursor::TransformComponent>();
	world.RegisterEvent<shared::cursor::ClientTransformEvent>();
}