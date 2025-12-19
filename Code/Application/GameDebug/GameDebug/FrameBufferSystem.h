#pragma once

#include "ECS/System.h"

namespace debug
{
	struct FrameBufferSingleton;
	struct FrameBufferWindowComponent;
	struct FrameBufferWindowRequest;
}

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	struct FrameBufferSingleton;
}

namespace debug
{
	class FrameBufferSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			debug::FrameBufferWindowComponent,
			// Events
			const debug::FrameBufferWindowRequest,
			// Singletons
			const eng::FrameBufferSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
};