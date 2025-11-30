#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace dbg
{
	struct BufferWindowRequest;
}

namespace eng
{
	struct FrameBufferSingleton;
}

namespace dbg
{
	struct FrameBufferSingleton;

	struct BufferWindowComponent final : public ecs::Component<BufferWindowComponent> { };

	class FrameBufferSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			dbg::BufferWindowComponent,
			const dbg::BufferWindowRequest,
			// Singletons
			const eng::FrameBufferSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
};