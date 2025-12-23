#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	struct MusicSingleton;
}

namespace eng::settings
{
	struct AudioSingleton;
}

namespace eng
{
	class MusicSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::MusicSingleton>
			::Read<
			eng::settings::AudioSingleton>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};