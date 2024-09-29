#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng::sound
{
	struct SequenceRequestComponent;
}

namespace hidden
{
	struct RevealComponent;
}

namespace hidden
{
	class SoundSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::sound::SequenceRequestComponent,
			const hidden::RevealComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::Entity m_SoundEntity = {};
	};
}