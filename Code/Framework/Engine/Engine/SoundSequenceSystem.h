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
}

namespace eng::sound
{
	struct SequenceComponent;
	struct SequenceBufferSingleton;
	struct SequenceRequestComponent;
}

namespace eng::sound
{
	class SequenceSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			// Components
			eng::sound::SequenceComponent,
			const eng::sound::SequenceRequestComponent,
			// Singletons
			eng::sound::SequenceBufferSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
};