#include "GameServerPCH.h"
#include "GameServer/UserSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationHost.h"
#include "ECS/WorldView.h"
#include "Engine/UserComponent.h"
#include "Engine/UserMapSingleton.h"
#include "Network/Host.h"
#include "Network/NetworkManager.h"

void net::UserSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	//auto& networkManager = world.WriteResource<net::NetworkManager>();
	//auto& adaptor = networkManager.GetAdaptor();

	//m_Collection =
	//{
	//	adaptor.m_OnServerClientConnected.Connect(*this, &net::UserSystem::OnClientConnected),
	//	adaptor.m_OnServerClientDisconnected.Connect(*this, &net::UserSystem::OnClientDisconnected),
	//};
}

void net::UserSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	//m_Collection.Disconnect();
}

void net::UserSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& userMapComponent = world.ReadSingleton<net::UserMapSingleton>();
	for (auto&& [userId, wantsConnected] : m_Requests)
	{
		auto& replicationHost = world.WriteResource<ecs::ReplicationHost>();

		const bool isConnected = userMapComponent.m_UserToEntity.Contains(userId);
		if (wantsConnected && !isConnected)
		{
			const ecs::Entity userEntity = world.CreateEntity();
			auto& nameComponent = world.AddComponent<ecs::NameComponent>(userEntity);
			nameComponent.m_Name = "User: ";
			nameComponent.m_Name += std::to_string(userId.GetInputId());
			nameComponent.m_Name += ", ";
			nameComponent.m_Name += std::to_string(userId.GetPeerId().GetIndex());

			auto& userComponent = world.AddComponent<net::UserComponent>(userEntity);
			userComponent.m_UserId = userId;

			auto& mapComponent = world.WriteSingleton<net::UserMapSingleton>();
			mapComponent.m_EntityToUser.Set(userEntity, userId);
			mapComponent.m_UserToEntity.Set(userId, userEntity);

			//replicationHost.RegisterPeer(userId.GetPeerId());
			//replicationHost.StartReplicateToPeer(userId.GetPeerId(), userEntity);
		}
		else if (!wantsConnected && isConnected)
		{
			const ecs::Entity userEntity = userMapComponent.m_UserToEntity.Get(userId);
			world.DestroyEntity(userEntity);

			auto& mapComponent = world.WriteSingleton<net::UserMapSingleton>();
			mapComponent.m_EntityToUser.Remove(userEntity);
			mapComponent.m_UserToEntity.Remove(userId);

			//replicationHost.StopReplicateToPeer(userId.GetPeerId(), userEntity);
			//replicationHost.UnregisterPeer(userId.GetPeerId());
		}
	}
	m_Requests.RemoveAll();
}

void net::UserSystem::OnClientConnected(const net::PeerId& peerId)
{
	net::UserId userId = net::UserId(0, peerId);
	m_Requests.Set(userId, true);
}

void net::UserSystem::OnClientDisconnected(const net::PeerId& peerId)
{
	net::UserId userId = net::UserId(0, peerId);
	m_Requests.Set(userId, false);
}
