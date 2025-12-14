#pragma once

#include "ECS/System.h"

namespace client::settings
{
	struct DebugSingleton;
}

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace hidden::settings
{
	struct DebugSingleton;
}

namespace debug::settings
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			client::settings::DebugSingleton,
			eng::settings::DebugSingleton,
			::hidden::settings::DebugSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};