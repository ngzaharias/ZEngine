#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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

namespace shared::camera
{
	struct SettingsComponent;
}

namespace client
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::settings::AudioComponent,
			eng::settings::GameplayComponent,
			eng::settings::WindowComponent,
			shared::camera::SettingsComponent>
			::Read<
			eng::WindowManager>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};