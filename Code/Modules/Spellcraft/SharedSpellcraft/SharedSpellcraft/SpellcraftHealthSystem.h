#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::spellcraft
{
	struct HealthComponent;
	struct HealthTemplate;
}

namespace shared::spellcraft
{
	class HealthSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::spellcraft::HealthComponent>
			::Read<
			shared::spellcraft::HealthTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}