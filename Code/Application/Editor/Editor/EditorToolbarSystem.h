#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::settings
{
	struct DebugSingleton;
}

namespace editor
{
	class ToolbarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::settings::DebugSingleton>
			::Read<>;

		void Update(World& world, const GameTime& gameTime);
	};
}
