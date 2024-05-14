#pragma once

#include <Core/Map.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

#include <Engine/SinkCollection.h>

#include <Network/UserId.h>

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class NetworkManager;
}

namespace net
{
	class Host;
	class ReplicationHost;
}

namespace net
{
	struct UserComponent;
	struct UserMapComponent;

	/// \brief Responsible for creating/destroying an entity/component per user that connects/disconnects.
	class UserSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::NetworkManager,
			net::UserComponent,
			net::UserMapComponent>;

		UserSystem(net::ReplicationHost& replicationHost);

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	private:
		net::ReplicationHost& m_ReplicationHost;

		SinkCollection m_Connections;

		Map<net::UserId, bool> m_Requests;
	};
}