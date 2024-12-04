#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct FlipbookAssetComponent;
	struct FlipbookComponent;

	class FlipbookAssetSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager, 
			eng::FlipbookAssetComponent, 
			const eng::FlipbookComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};