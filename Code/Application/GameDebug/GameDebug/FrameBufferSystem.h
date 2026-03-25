#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct FrameBufferComponent;
	struct FrameBufferWindowComponent;
	struct FrameBufferWindowEvent;
}

namespace eng
{
	struct FrameBufferComponent;
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
			debug::FrameBufferWindowEvent,
			eng::FrameBufferComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};