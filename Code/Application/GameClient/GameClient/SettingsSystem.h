#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::settings::AudioSingleton,
			eng::settings::CameraSingleton,
			eng::settings::GameplaySingleton,
			eng::settings::WindowSingleton>
			::Read<
			eng::WindowManager>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};