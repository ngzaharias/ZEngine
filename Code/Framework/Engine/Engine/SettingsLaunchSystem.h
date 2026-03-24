#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::level
{
	struct LoadEvent;
}

namespace eng::settings
{
	struct LaunchStaticComponent;
}

namespace eng::settings
{
	class LaunchSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::level::LoadEvent,
			eng::settings::LaunchStaticComponent>;

		void Initialise(World& world);
	};
};