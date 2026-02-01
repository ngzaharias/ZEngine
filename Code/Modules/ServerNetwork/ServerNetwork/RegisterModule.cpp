#include "ServerNetworkPCH.h"
#include "ServerNetwork/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ServerNetwork/NetworkPeerComponent.h"
#include "ServerNetwork/NetworkPeerSystem.h"

void server::network::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<server::network::PeerComponent>();
	}

	// systems
	{
		world.RegisterSystem<server::network::PeerSystem>();
	}
}