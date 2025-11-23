#pragma once

#include "ECS/System.h"

namespace eng
{
	class AssetManager;
	struct FlipbookComponent;

	class FlipbookPlaySystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::AssetManager,
			// Components
			eng::FlipbookComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};