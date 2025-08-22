#pragma once

#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class AssetManager;
}

namespace eng::settings
{
	struct AudioComponent;
}

namespace eng::sound
{
	struct ObjectComponent;
	struct RandomBufferComponent;
	struct SequenceBufferComponent;
	struct SingleRequestComponent;
}

namespace eng::sound
{
	class PlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			ecs::NameComponent,
			eng::sound::ObjectComponent,
			const eng::settings::AudioComponent,
			const eng::sound::RandomBufferComponent,
			const eng::sound::SequenceBufferComponent,
			const eng::sound::SingleRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};