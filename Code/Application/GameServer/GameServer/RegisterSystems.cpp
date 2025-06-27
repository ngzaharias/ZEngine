#include "GameServerPCH.h"
#include "GameServer/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/ReplicationHost.h"
#include "GameServer/ReplicationSystem.h"
#include "GameServer/UserSystem.h"

void svr::RegisterSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<net::ReplicationSystem>();
	entityWorld.RegisterSystem<net::UserSystem>();
}