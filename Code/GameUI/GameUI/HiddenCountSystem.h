#pragma once

#include "ECS/System.h"

namespace hidden
{
	struct CountComponent;
}

namespace gui::hidden
{
	class CountSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			const ::hidden::CountComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}