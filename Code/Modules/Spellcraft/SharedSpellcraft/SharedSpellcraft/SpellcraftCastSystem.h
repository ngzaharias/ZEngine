#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gamestate
{
	struct EditModeComponent;
}

namespace spellcraft
{
	struct BookComponent;
	struct CastComponent;
	struct ExplosionComponent;
	struct ExplosionHitEvent;
	struct ProjectileComponent;
	struct ProjectileHitEvent;
}

namespace spellcraft
{
	class CastSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			spellcraft::CastComponent>
			::Read<
			gamestate::EditModeComponent,
			spellcraft::BookComponent,
			spellcraft::ExplosionComponent,
			spellcraft::ExplosionHitEvent,
			spellcraft::ProjectileComponent,
			spellcraft::ProjectileHitEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}