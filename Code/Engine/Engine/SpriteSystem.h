#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct SpriteAssetComponent;
	struct SpriteComponent;
}

namespace eng
{
	class SpriteSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::SpriteAssetComponent,
			const eng::SpriteComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}