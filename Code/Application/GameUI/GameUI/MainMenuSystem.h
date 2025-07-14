#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
}

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
	struct MenuComponent;
}

namespace gui::main_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			eng::application::CloseRequestComponent,
			eng::level::LoadRequestComponent,
			gui::main_menu::MenuComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}