#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

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
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			// Singletons
			eng::MusicSingleton,
			const eng::settings::AudioSingleton>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};