#pragma once

#include "Core/Delegate.h"
#include "Core/Set.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Network/PeerId.h"

namespace ecs
{
	struct ReplicationComponent;
}

namespace server::cursor
{
	struct TransformStaticComponent;
}

namespace server::network
{
	struct PeerComponent;
}

namespace shared::cursor
{
	struct ClientTransformEvent;
	struct TransformComponent;
}

namespace server::cursor
{
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::ReplicationComponent,
			server::cursor::TransformStaticComponent,
			shared::cursor::TransformComponent>
			::Read<
			server::network::PeerComponent,
			shared::cursor::ClientTransformEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}