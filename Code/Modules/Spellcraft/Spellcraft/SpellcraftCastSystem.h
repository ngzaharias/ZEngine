#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
}

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
			eng::InputManager,
			spellcraft::CastComponent>
			::Read<
			gamestate::EditModeComponent,
			spellcraft::BookComponent,
			spellcraft::ExplosionComponent,
			spellcraft::ExplosionHitEvent,
			spellcraft::ProjectileComponent,
			spellcraft::ProjectileHitEvent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}