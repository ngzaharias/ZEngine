#pragma once

#include "ECS/System.h"

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct AudioSingleton;
	struct CameraSingleton;
	struct GameplaySingleton;
	struct WindowSingleton;
}

namespace client
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Singletons
			eng::settings::AudioSingleton,
			eng::settings::CameraSingleton,
			eng::settings::GameplaySingleton,
			eng::settings::WindowSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};