#pragma once

#include "ECS/System.h"

namespace edit::settings
{
	struct LocalComponent;

	class LocalSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			edit::settings::LocalComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};