#pragma once

#include "Core/Guid.h"
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
	struct AudioSingleton;
}

namespace eng::sound
{
	struct ObjectComponent;
	struct RandomBufferSingleton;
	struct SequenceBufferSingleton;
	struct SingleRequestComponent;
}

namespace eng::sound
{
	class PlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			// Components
			ecs::NameComponent,
			eng::sound::ObjectComponent,
			const eng::sound::SingleRequestComponent,
			// Singletons
			const eng::settings::AudioSingleton,
			const eng::sound::RandomBufferSingleton,
			const eng::sound::SequenceBufferSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		Array<str::Guid> m_Requests = {};
	};
};