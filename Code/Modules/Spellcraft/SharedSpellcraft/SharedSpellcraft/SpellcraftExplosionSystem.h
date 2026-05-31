#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct FlipbookComponent;
	struct TransformComponent;
}

namespace shared::spellcraft
{
	struct BookComponent;
	struct CastComponent;
	struct ExplosionComponent;
	struct ExplosionHitEvent;
}

namespace shared::spellcraft
{
	class ExplosionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::FlipbookComponent,
			eng::TransformComponent,
			shared::spellcraft::ExplosionComponent,
			shared::spellcraft::ExplosionHitEvent>
			::Read<
			shared::spellcraft::BookComponent,
			shared::spellcraft::CastComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}