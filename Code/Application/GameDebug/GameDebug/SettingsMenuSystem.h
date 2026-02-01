#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace client::hidden
{
	struct DebugSingleton;
}

namespace client::settings
{
	struct DebugSingleton;
}

namespace debug::settings
{
	struct WindowComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace debug::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::DebugSingleton,
			client::settings::DebugSingleton,
			debug::settings::WindowComponent,
			ecs::NameComponent,
			eng::settings::DebugSingleton,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}