#include "ServerNetworkPCH.h"
#include "ServerNetwork/NetworkRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ServerNetwork/NetworkPeerComponent.h"
#include "ServerNetwork/NetworkPeerSystem.h"

void server::network::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<server::network::PeerComponent>();
	world.RegisterSystem<server::network::PeerSystem>();
}