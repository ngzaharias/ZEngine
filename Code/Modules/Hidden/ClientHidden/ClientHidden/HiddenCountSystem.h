#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::hidden
{
	struct CountComponent;
	struct CountTemplate;
	struct GroupComponent;
	struct GroupTemplate;
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
			client::hidden::CountTemplate,
			client::hidden::GroupComponent,
			client::hidden::GroupTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}