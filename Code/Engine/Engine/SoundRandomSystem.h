#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
}

namespace eng::sound
{
	struct RandomComponent;
	struct RandomBufferComponent;
	struct RandomRequestComponent;
}

namespace eng::sound
{
	class RandomSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			eng::sound::RandomComponent,
			eng::sound::RandomBufferComponent,
			const eng::sound::RandomRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};