#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct SettingsComponent;
}

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct AudioComponent;
	struct GameplayComponent;
	struct WindowComponent;
}

namespace client
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			camera::SettingsComponent,
			eng::settings::AudioComponent,
			eng::settings::GameplayComponent,
			eng::settings::WindowComponent>
			::Read<
			eng::WindowManager>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};