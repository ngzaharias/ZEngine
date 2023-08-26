#pragma once

#include <ECS/Component.h>
#include <ECS/System.h>

namespace dbg
{
	struct BufferWindowRequestComponent;
}

namespace eng
{
	struct FrameBufferComponent;
}

namespace dbg
{
	struct FrameBufferComponent;

	struct BufferWindowComponent : public ecs::Component<BufferWindowComponent> { };

	class FrameBufferSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::BufferWindowComponent,
			const dbg::BufferWindowRequestComponent,
			const eng::FrameBufferComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};