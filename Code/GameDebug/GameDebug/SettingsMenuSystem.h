#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace clt::settings
{
	struct DebugComponent;
}

namespace dbg::settings
{
	struct RequestComponent;
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

namespace hidden::settings
{
	struct DebugComponent;
}

namespace dbg::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::WindowManager,
			// Components
			ecs::NameComponent,
			clt::settings::DebugComponent,
			dbg::settings::WindowComponent,
			eng::settings::DebugComponent,
			::hidden::settings::DebugComponent,
			const dbg::settings::RequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}