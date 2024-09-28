#include "GameServerPCH.h"
#include "GameServer/UserSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/NetworkManager.h"
#include "Engine/ReplicationHost.h"
#include "Engine/UserComponents.h"
#include "Network/Adaptor.h"
#include "Network/Host.h"

void net::UserSystem::Initialise(World& world)
{
	auto& networkManager = world.WriteResource<eng::NetworkManager>();
	auto& adaptor = networkManager.GetAdaptor();

	m_Connections =
	{
		entt::sink(adaptor.m_OnServerClientConnected).connect<&net::UserSystem::OnClientConnected>(this),
		entt::sink(adaptor.m_OnServerClientDisconnected).connect<&net::UserSystem::OnClientDisconnected>(this),
	};
}

void net::UserSystem::Shutdown(World& world)
{
	m_Connections.Disconnect();
}

void net::UserSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& userMapComponent = world.ReadSingleton<net::UserMapComponent>();
	for (auto&& [userId, wantsConnected] : m_Requests)
	{
		auto& replicationHost = world.WriteResource<net::ReplicationHost>();

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

			auto& mapComponent = world.WriteSingleton<net::UserMapComponent>();
			mapComponent.m_EntityToUser.Set(userEntity, userId);
			mapComponent.m_UserToEntity.Set(userId, userEntity);

			replicationHost.RegisterPeer(userId.GetPeerId());
			replicationHost.StartReplicateToPeer(userId.GetPeerId(), userEntity);
		}
		else if (!wantsConnected && isConnected)
		{
			const ecs::Entity userEntity = userMapComponent.m_UserToEntity.Get(userId);
			world.DestroyEntity(userEntity);

			auto& mapComponent = world.WriteSingleton<net::UserMapComponent>();
			mapComponent.m_EntityToUser.Remove(userEntity);
			mapComponent.m_UserToEntity.Remove(userId);

			replicationHost.StopReplicateToPeer(userId.GetPeerId(), userEntity);
			replicationHost.UnregisterPeer(userId.GetPeerId());
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
