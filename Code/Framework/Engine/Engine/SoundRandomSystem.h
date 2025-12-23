#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::sound::RandomBufferSingleton,
			eng::sound::RandomComponent>
			::Read<
			eng::sound::RandomRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};