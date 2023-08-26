#include "GameServerPCH.h"
#include "GameServer/ReplicationSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/ReplicationComponents.h>
#include <Engine/ReplicationHost.h>

net::ReplicationSystem::ReplicationSystem(net::ReplicationHost& replicationHost)
	: m_ReplicationHost(replicationHost)
{
}

void net::ReplicationSystem::Update(World& world, const GameTime& gameTime)
{

}