#pragma once

#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class AssetManager;
	struct MusicComponent;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace eng
{
	class MusicSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// resources
			eng::AssetManager,
			// components
			ecs::NameComponent,
			eng::MusicComponent,
			const eng::settings::LocalComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};