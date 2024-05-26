#pragma once

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace eng
{
	struct SpriteComponent;
}

namespace eng::sound
{
	struct SequenceRequestComponent;
}

namespace hidden
{
	struct ObjectComponent;
	struct RevealedComponent;
}

namespace hidden
{
	class SoundSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::sound::SequenceRequestComponent,
			const hidden::ObjectComponent,
			const hidden::RevealedComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::Entity m_SoundEntity = {};
	};
}