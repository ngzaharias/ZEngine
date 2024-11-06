#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct SpriteComponent;
	struct SpriteRequestComponent;
}

namespace eng
{
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::SpriteComponent,
			const eng::SpriteRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}