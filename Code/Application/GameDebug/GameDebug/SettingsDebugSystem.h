#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct DebugStaticComponent;
}

namespace client::settings
{
	struct DebugStaticComponent;
}

namespace eng::settings
{
	struct DebugStaticComponent;
}

namespace debug::settings
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::DebugStaticComponent,
			client::settings::DebugStaticComponent,
			eng::settings::DebugStaticComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};