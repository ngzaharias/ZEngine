#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::level
{
	struct LoadEvent;
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
		using World = ecs::WorldView
			::Write<
			eng::level::LoadEvent,
			eng::settings::LaunchSingleton>;

		void Initialise(World& world);
	};
};