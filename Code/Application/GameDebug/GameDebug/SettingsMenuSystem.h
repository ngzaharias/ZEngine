#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

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

namespace hidden::settings
{
	struct DebugSingleton;
}

namespace debug::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::WindowManager,
			// Components
			ecs::NameComponent,
			debug::settings::WindowComponent,
			// Singletons
			client::settings::DebugSingleton,
			eng::settings::DebugSingleton,
			::hidden::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}