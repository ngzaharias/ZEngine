#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace debug
{
	struct BufferWindowRequest;
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
	struct FrameBufferSingleton;

	struct BufferWindowComponent final : public ecs::Component<BufferWindowComponent> { };

	class FrameBufferSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			debug::BufferWindowComponent,
			const debug::BufferWindowRequest,
			// Singletons
			const eng::FrameBufferSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
};