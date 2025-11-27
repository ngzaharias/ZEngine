#pragma once

#include "ECS/System.h"

namespace clt::settings
{
	struct DebugSingleton;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace hidden::settings
{
	struct DebugSingleton;
}

namespace dbg::settings
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			clt::settings::DebugSingleton,
			eng::settings::DebugSingleton,
			::hidden::settings::DebugSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};