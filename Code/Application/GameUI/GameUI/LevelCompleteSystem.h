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
	struct LoadRequest;
}

namespace eng::settings
{
	struct LaunchComponent;
}

namespace gui::level_complete
{
	struct ExitGameRequest;
	struct ExitToMenuRequest;
	struct WindowComponent;
}

namespace gui::level_complete
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			// Components
			eng::application::CloseRequestComponent,
			eng::level::LoadRequest,
			const eng::settings::LaunchComponent,
			const gui::level_complete::ExitGameRequest,
			const gui::level_complete::ExitToMenuRequest,
			const gui::level_complete::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}