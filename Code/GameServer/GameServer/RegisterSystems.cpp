#include "GameServerPCH.h"
#include "GameServer/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/ReplicationHost.h"
#include "GameServer/ReplicationSystem.h"
#include "GameServer/UserSystem.h"

void svr::RegisterSystems(ecs::EntityWorld& entityWorld, const svr::SystemDependencies& dependencies)
{
	entityWorld.RegisterSystem<net::ReplicationSystem>(
		dependencies.m_ReplicationHost);
	entityWorld.RegisterSystem<net::UserSystem>(
		dependencies.m_ReplicationHost);
}