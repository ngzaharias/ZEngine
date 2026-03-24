#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	struct MusicStaticComponent;
}

namespace eng::settings
{
	struct AudioStaticComponent;
}

namespace eng
{
	class MusicSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::MusicStaticComponent>
			::Read<
			eng::settings::AudioStaticComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};