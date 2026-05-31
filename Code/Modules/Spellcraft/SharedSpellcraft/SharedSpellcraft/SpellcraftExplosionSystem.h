#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct FlipbookComponent;
	struct TransformComponent;
}

namespace spellcraft
{
	struct BookComponent;
	struct CastComponent;
	struct ExplosionComponent;
	struct ExplosionHitEvent;
}

namespace spellcraft
{
	class ExplosionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::FlipbookComponent,
			eng::TransformComponent,
			spellcraft::ExplosionComponent,
			spellcraft::ExplosionHitEvent>
			::Read<
			spellcraft::BookComponent,
			spellcraft::CastComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}