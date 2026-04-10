#include "DebugEntityPCH.h"
#include "DebugEntity/EntityRegisterModule.h"

#include "DebugEntity/EntityWindowComponent.h"
#include "DebugEntity/EntityWindowEvent.h"
#include "DebugEntity/EntityWindowSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"

void debug::entity::RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
{
	clientWorld.RegisterComponent<debug::entity::WindowComponent>();
	clientWorld.RegisterEvent<debug::entity::WindowEvent>();
	clientWorld.RegisterSystem<debug::entity::WindowSystem>(clientWorld, serverWorld);
}