#pragma once

#include "Core/Delegate.h"
#include "Core/Set.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Network/PeerId.h"

namespace server::cursor
{
	struct TransformSingleton;
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
			server::cursor::TransformSingleton,
			shared::cursor::TransformComponent>
			::Read<
			shared::cursor::ClientTransformEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}