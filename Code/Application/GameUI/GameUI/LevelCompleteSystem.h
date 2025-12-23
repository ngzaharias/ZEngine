#pragma once

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
		using World = ecs::WorldView
			::Write<
			eng::application::CloseRequest,
			eng::level::LoadRequest,
			eng::UIManager>
			::Read<
			eng::level::LoadedComponent,
			eng::settings::LaunchSingleton,
			gui::level_complete::ExitGameRequest,
			gui::level_complete::ExitToMenuRequest,
			gui::level_complete::ResetGameRequest,
			gui::level_complete::WindowComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}