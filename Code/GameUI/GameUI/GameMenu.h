#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng::application
{
	struct CloseRequestComponent;
}

namespace eng::level
{
	struct LoadRequestComponent;
}

namespace eng::settings
{
	struct LaunchComponent;
}

namespace gui::game_menu
{
	struct OpenRequestComponent;
	struct WindowComponent;
}

namespace gui::settings
{
	struct OpenRequestComponent;
}

namespace gui::game_menu
{
	class GameMenu final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::application::CloseRequestComponent,
			eng::level::LoadRequestComponent,
			gui::settings::OpenRequestComponent,
			const eng::settings::LaunchComponent,
			const gui::game_menu::OpenRequestComponent,
			const gui::game_menu::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}