#include "ReplicationPCH.h"
#include "ServerReplication/ReplicationRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ServerReplication/ReplicationManagerSystem.h"

void server::replication::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterSystem<server::replication::ManagerSystem>();
}