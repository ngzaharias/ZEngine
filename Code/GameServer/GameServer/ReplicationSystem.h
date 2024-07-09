#pragma once

#include "ECS/System.h"

namespace net
{
	class ReplicationHost;
}

namespace net
{
	struct HostReplicationComponent;

	class ReplicationSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<net::HostReplicationComponent>;

		ReplicationSystem(net::ReplicationHost& replicationHost);

		void Update(World& world, const GameTime& gameTime);

	private:
		net::ReplicationHost& m_ReplicationHost;
	};
}