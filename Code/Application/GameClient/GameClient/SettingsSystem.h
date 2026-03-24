#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct SettingsStaticComponent;
}

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct AudioStaticComponent;
	struct GameplayStaticComponent;
	struct WindowStaticComponent;
}

namespace client
{
	class SettingsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			camera::SettingsStaticComponent,
			eng::settings::AudioStaticComponent,
			eng::settings::GameplayStaticComponent,
			eng::settings::WindowStaticComponent>
			::Read<
			eng::WindowManager>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};