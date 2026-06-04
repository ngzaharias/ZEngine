#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::tilemap
{
	struct GridComponent;
	struct GridTemplate;
}

namespace server::tilemap
{
	class GridSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::tilemap::GridComponent>
			::Read<
			shared::tilemap::GridTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}