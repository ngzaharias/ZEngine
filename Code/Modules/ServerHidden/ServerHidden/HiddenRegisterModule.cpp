#include "ServerHiddenPCH.h"
#include "ServerHidden/HiddenRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ServerHidden/HiddenRevealSystem.h"

void server::hidden::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterSystem<server::hidden::RevealSystem>();
}