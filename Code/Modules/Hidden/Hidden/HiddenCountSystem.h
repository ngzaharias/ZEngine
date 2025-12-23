#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace hidden
{
	struct CountComponent;
	struct GroupComponent;
}

namespace hidden
{
	class CountSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			::hidden::CountComponent>
			::Read<
			::hidden::GroupComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}