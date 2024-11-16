#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct SpriteAssetComponent;
	struct SpriteComponent;
}

namespace hexmap
{
	struct AssetComponent;
	struct FragmentComponent;
}

namespace hexmap
{
	class AssetSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			hexmap::AssetComponent,
			const hexmap::FragmentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}