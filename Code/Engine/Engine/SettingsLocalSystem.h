#pragma once

#include "ECS/System.h"

namespace eng::settings
{
	struct LocalComponent;

	class LocalSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::settings::LocalComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};