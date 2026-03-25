#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace client::hidden
{
	struct DebugComponent;
}

namespace client::settings
{
	struct DebugComponent;
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
	struct DebugComponent;
}

namespace debug::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::DebugComponent,
			client::settings::DebugComponent,
			debug::settings::WindowComponent,
			ecs::NameComponent,
			eng::settings::DebugComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}