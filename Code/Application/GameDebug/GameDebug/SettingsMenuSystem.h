#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace client::hidden
{
	struct DebugStaticComponent;
}

namespace client::settings
{
	struct DebugStaticComponent;
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
	struct DebugStaticComponent;
}

namespace debug::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::DebugStaticComponent,
			client::settings::DebugStaticComponent,
			debug::settings::WindowComponent,
			ecs::NameComponent,
			eng::settings::DebugStaticComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}