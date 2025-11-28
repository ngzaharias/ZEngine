#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct LoadRequestEvent;
}

namespace eng::settings
{
	struct LaunchSingleton;
}

namespace eng::settings
{
	class LaunchSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Events
			eng::level::LoadRequestEvent,
			// Singletons
			eng::settings::LaunchSingleton>;

		void Initialise(World& world);
	};
};