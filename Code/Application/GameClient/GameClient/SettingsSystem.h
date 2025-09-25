#pragma once

#include "ECS/System.h"

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct AudioComponent;
	struct CameraComponent;
	struct GameplayComponent;
	struct WindowComponent;
}

namespace client
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			const eng::WindowManager,
			// components
			eng::settings::AudioComponent,
			eng::settings::CameraComponent,
			eng::settings::GameplayComponent,
			eng::settings::WindowComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};