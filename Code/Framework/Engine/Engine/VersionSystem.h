#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct VersionComponent;
}

namespace eng
{
	class VersionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::VersionComponent>;

		void Initialise(World& world);
	};
}