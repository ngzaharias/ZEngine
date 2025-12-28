#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct WindowSingleton;
}

namespace eng
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::WindowManager>
			::Read<
			eng::settings::WindowSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}