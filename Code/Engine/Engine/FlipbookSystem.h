#pragma once

#include <ECS/System.h>

namespace eng
{
	class AssetManager;
	struct FlipbookComponent;

	class FlipbookSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<eng::AssetManager, eng::FlipbookComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};