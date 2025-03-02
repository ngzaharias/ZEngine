#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::application
{
	struct CloseRequestComponent;
}

namespace eng::level
{
	struct LoadRequestComponent;
}

namespace gui::main_menu
{
	struct WindowComponent;
}

namespace gui::settings
{
	struct OpenRequestComponent;
}

namespace gui::main_menu
{
	class MainMenu final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::application::CloseRequestComponent,
			eng::level::LoadRequestComponent,
			gui::settings::OpenRequestComponent,
			const gui::main_menu::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}