#pragma once

#include <ECS/System.h>

namespace eng
{
	class AssetManager;
}

namespace eng::sound
{
	struct SequenceComponent;
	struct SequenceBufferComponent;
	struct SequenceRequestComponent;
}

namespace eng::sound
{
	class SequenceSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// managers
			eng::AssetManager,
			// components
			eng::sound::SequenceComponent,
			eng::sound::SequenceBufferComponent,
			const eng::sound::SequenceRequestComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
};