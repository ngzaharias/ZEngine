#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace net
{
	class ReplicationHost;
	struct HostReplicationComponent;
}

namespace net
{
	class ReplicationSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			net::ReplicationHost,
			// Components
			net::HostReplicationComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}