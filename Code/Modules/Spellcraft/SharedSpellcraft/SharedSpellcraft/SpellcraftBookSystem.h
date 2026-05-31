#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::spellcraft
{
	struct BookComponent;
}

namespace shared::spellcraft
{
	class BookSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::spellcraft::BookComponent>
			::Read<>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}