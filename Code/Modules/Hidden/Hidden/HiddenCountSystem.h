#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

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
		using World = ecs::WorldView<
			::hidden::CountComponent,
			const ::hidden::GroupComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}