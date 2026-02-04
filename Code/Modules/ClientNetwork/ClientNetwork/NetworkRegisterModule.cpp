#include "ClientNetworkPCH.h"
#include "ClientNetwork/NetworkRegisterModule.h"

#include "ClientNetwork/NetworkPeerComponent.h"
#include "ClientNetwork/NetworkPeerSystem.h"
#include "ECS/EntityWorld.h"

void client::network::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<client::network::PeerComponent>();
	}

	// systems
	{
		world.RegisterSystem<client::network::PeerSystem>();
	}
}