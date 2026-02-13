#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
}

namespace editor
{
	class ToolbarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<>
			::Read<>;

		void Update(World& world, const GameTime& gameTime);
	};
}
