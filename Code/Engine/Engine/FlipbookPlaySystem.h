#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct FlipbookAssetComponent;
	struct FlipbookComponent;

	class FlipbookPlaySystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::FlipbookComponent,
			const eng::FlipbookAssetComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};