#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct LoadRequest;
}

namespace eng::settings
{
	struct LaunchComponent;

	class LaunchSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequest,
			eng::settings::LaunchComponent>;

		void Initialise(World& world);
	};
};