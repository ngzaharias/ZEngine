#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct SpriteComponent;
	struct TransformComponent;
}

namespace spellcraft
{
	struct BookComponent;
	struct CastComponent;
	struct ProjectileComponent;
	struct ProjectileHitEvent;
}

namespace spellcraft
{
	class ProjectileSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::SpriteComponent,
			eng::TransformComponent,
			spellcraft::ProjectileComponent,
			spellcraft::ProjectileHitEvent>
			::Read<
			spellcraft::BookComponent,
			spellcraft::CastComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}