#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Network/UserId.h"

namespace ecs
{
	class ReplicationHost;
	struct NameComponent;
}

namespace net
{
	class Host;
	struct UserComponent;
	struct UserMapSingleton;
}

namespace net
{
	/// \brief Responsible for creating/destroying an entity/component per user that connects/disconnects.
	class UserSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			ecs::ReplicationHost,
			net::Host,
			net::UserComponent,
			net::UserMapSingleton>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	private:
		Map<net::UserId, bool> m_Requests;
	};
}