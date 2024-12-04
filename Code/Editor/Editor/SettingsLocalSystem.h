#pragma once

#include "ECS/System.h"

namespace editor::settings
{
	struct LocalComponent;

	class LocalSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			editor::settings::LocalComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};