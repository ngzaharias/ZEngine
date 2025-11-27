#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Map.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "Network/UserId.h"

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
	struct UserComponent;
	struct UserMapSingleton;
}

namespace net
{
	/// \brief Responsible for creating/destroying an entity/component per user that connects/disconnects.
	class UserSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			net::ReplicationHost,
			// Components
			ecs::NameComponent,
			eng::NetworkManager,
			net::UserComponent,
			// Singletons
			net::UserMapSingleton>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	private:
		DelegateCollection m_Collection = { };
		Map<net::UserId, bool> m_Requests;
	};
}