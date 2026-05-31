#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace gamestate
{
	struct EditModeComponent;
}

namespace shared::spellcraft
{
	struct BookComponent;
	struct CastComponent;
	struct ExplosionComponent;
	struct ExplosionHitEvent;
	struct ProjectileComponent;
	struct ProjectileHitEvent;
}

namespace shared::spellcraft
{
	class CastSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::spellcraft::CastComponent>
			::Read<
			gamestate::EditModeComponent,
			shared::spellcraft::BookComponent,
			shared::spellcraft::ExplosionComponent,
			shared::spellcraft::ExplosionHitEvent,
			shared::spellcraft::ProjectileComponent,
			shared::spellcraft::ProjectileHitEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}