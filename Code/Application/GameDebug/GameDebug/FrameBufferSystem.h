#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct FrameBufferSingleton;
	struct FrameBufferWindowComponent;
	struct FrameBufferWindowRequest;
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
		using World = ecs::WorldView
			::Write<
			debug::FrameBufferWindowComponent>
			::Read<
			debug::FrameBufferWindowRequest,
			eng::FrameBufferSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
};