#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	struct FlipbookComponent;

	class FlipbookPlaySystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::FlipbookComponent>
			::Read<
			eng::AssetManager>;

		void Update(World& world, const GameTime& gameTime);
	};
};