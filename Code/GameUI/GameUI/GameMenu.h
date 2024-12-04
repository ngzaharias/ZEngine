#pragma once

#include "ECS/System.h"

namespace gui
{
	class GameMenu final : public ecs::System
	{
	public:
		using World = ecs::WorldView<>;

		void Update(World& world, const GameTime& gameTime);
	};
}