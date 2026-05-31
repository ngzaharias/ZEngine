#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct SpriteComponent;
	struct TransformComponent;
}

namespace shared::spellcraft
{
	struct BookComponent;
	struct CastComponent;
	struct ProjectileComponent;
	struct ProjectileHitEvent;
}

namespace shared::spellcraft
{
	class ProjectileSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent,
			eng::TransformComponent,
			shared::spellcraft::ProjectileComponent,
			shared::spellcraft::ProjectileHitEvent>
			::Read<
			shared::spellcraft::BookComponent,
			shared::spellcraft::CastComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}