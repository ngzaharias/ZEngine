#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct CountComponent;
	struct GroupComponent;
}

namespace client::hidden
{
	class CountSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			client::hidden::CountComponent>
			::Read<
			client::hidden::GroupComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}