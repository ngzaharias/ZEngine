#pragma once

#include "ECS/System.h"

namespace eng
{
	struct VersionSingleton;
}

namespace eng
{
	class VersionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Singletons
			eng::VersionSingleton>;

		void Initialise(World& world);
	};
}