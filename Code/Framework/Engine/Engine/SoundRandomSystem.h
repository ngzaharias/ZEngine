#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
}

namespace eng::sound
{
	struct RandomComponent;
	struct RandomBufferSingleton;
	struct RandomRequestComponent;
}

namespace eng::sound
{
	class RandomSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			// Components
			eng::sound::RandomComponent,
			const eng::sound::RandomRequestComponent,
			// Singletons
			eng::sound::RandomBufferSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
};