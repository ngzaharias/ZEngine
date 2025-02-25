#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct LoadRequestComponent;
}

namespace eng::settings
{
	struct LaunchComponent;

	class LaunchSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequestComponent,
			eng::settings::LaunchComponent>;

		void Initialise(World& world);
	};
};