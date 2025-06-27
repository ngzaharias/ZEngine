#include "GameServerPCH.h"
#include "GameServer/ReplicationSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ReplicationComponents.h"
#include "Engine/ReplicationHost.h"

void net::ReplicationSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

}