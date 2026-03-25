#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct DebugComponent;
}

namespace client::settings
{
	struct DebugComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace debug::settings
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::DebugComponent,
			client::settings::DebugComponent,
			eng::settings::DebugComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};