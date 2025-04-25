#pragma once

#include "ECS/System.h"

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace eng::settings
{
	class LocalSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			const eng::WindowManager,
			// components
			eng::settings::LocalComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};