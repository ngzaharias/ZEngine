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
	struct CloseRequest;
}

namespace eng::level
{
	struct LoadedComponent;
	struct LoadRequest;
}

namespace eng::settings
{
	struct LaunchSingleton;
}

namespace gui::level_complete
{
	struct ExitGameRequest;
	struct ExitToMenuRequest;
	struct ResetGameRequest;
	struct WindowComponent;
}

namespace gui::level_complete
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView <
			// Resources
			eng::UIManager,
			// Components
			eng::level::LoadRequest,
			const eng::level::LoadedComponent,
			const gui::level_complete::WindowComponent,
			// Events
			eng::application::CloseRequest,
			const gui::level_complete::ExitGameRequest,
			const gui::level_complete::ExitToMenuRequest,
			const gui::level_complete::ResetGameRequest,
			// Singletons
			const eng::settings::LaunchSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}