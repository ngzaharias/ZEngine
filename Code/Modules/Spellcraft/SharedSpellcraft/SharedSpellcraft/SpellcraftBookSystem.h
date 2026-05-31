#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace spellcraft
{
	struct BookComponent;
}

namespace spellcraft
{
	class BookSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			spellcraft::BookComponent>
			::Read<>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}