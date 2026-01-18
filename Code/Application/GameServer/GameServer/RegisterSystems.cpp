#include "GameServerPCH.h"
#include "GameServer/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "GameServer/ReplicationSystem.h"
#include "GameServer/UserSystem.h"

void server::RegisterSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<net::ReplicationSystem>();
	entityWorld.RegisterSystem<net::UserSystem>();
}