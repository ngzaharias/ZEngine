#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct RevealComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct SavegameComponent;
}

namespace eng::sound
{
	struct SequenceRequestComponent;
}

namespace client::hidden
{
	class SoundSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::sound::SequenceRequestComponent>
			::Read<
			client::hidden::RevealComponent,
			eng::SavegameComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::Entity m_SoundEntity = {};
	};
}