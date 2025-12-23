#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct VersionSingleton;
}

namespace eng
{
	class VersionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::VersionSingleton>;

		void Initialise(World& world);
	};
}