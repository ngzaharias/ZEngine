#pragma once

#include "ECS/System.h"

namespace clt::settings
{
	struct DebugComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace dbg::settings
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			clt::settings::DebugComponent,
			eng::settings::DebugComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};