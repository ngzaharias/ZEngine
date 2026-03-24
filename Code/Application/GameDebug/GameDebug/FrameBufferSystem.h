#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct FrameBufferStaticComponent;
	struct FrameBufferWindowComponent;
	struct FrameBufferWindowEvent;
}

namespace eng
{
	struct FrameBufferStaticComponent;
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
			eng::FrameBufferStaticComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};