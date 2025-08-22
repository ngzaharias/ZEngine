#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
	class WindowManager;
}

namespace eng::settings
{
	struct AudioComponent;
	struct CameraComponent;
	struct WindowComponent;
}

namespace gui::settings_menu
{
	struct CloseRequest;
	struct OpenRequest;
	struct ValueRequest;
	struct WindowComponent;
}

namespace gui::settings_menu
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::UIManager,
			eng::WindowManager,
			// Components
			eng::settings::AudioComponent,
			eng::settings::CameraComponent,
			eng::settings::WindowComponent,
			gui::settings_menu::WindowComponent,
			const gui::settings_menu::CloseRequest,
			const gui::settings_menu::OpenRequest,
			const gui::settings_menu::ValueRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}