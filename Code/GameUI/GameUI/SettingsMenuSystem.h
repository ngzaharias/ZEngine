#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace clt::settings
{
	struct DebugComponent;
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
	struct LocalComponent;
	struct GraphicsComponent;
}

namespace gui::settings
{
	struct CloseRequestComponent;
	struct OpenRequestComponent;
	struct WindowComponent;
}

namespace hidden::settings
{
	struct DebugComponent;
}

namespace gui::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::WindowManager,
			// Components
			ecs::NameComponent,
			::clt::settings::DebugComponent,
			::eng::settings::DebugComponent,
			::eng::settings::LocalComponent,
			::eng::settings::GraphicsComponent,
			::gui::settings::WindowComponent,
			::hidden::settings::DebugComponent,
			const gui::settings::CloseRequestComponent,
			const gui::settings::OpenRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}