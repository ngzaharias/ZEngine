#pragma once

#include "ECS/System.h"

namespace eng::settings
{
	struct DebugComponent;

	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::settings::DebugComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};