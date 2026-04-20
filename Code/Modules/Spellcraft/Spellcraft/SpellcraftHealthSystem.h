#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace spellcraft
{
	struct HealthComponent;
	struct HealthTemplate;
}

namespace spellcraft
{
	class HealthSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			spellcraft::HealthComponent>
			::Read<
			spellcraft::HealthTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}