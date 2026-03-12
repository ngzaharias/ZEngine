#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
}

namespace icon
{
	class LoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>;

		void Initialise(World& world);
		void Shutdown(World& world);
	};
}
