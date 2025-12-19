#pragma once

#include "ECS/System.h"

namespace debug
{
	struct ImNodesDemoRequest;
	struct ImNodesDemoWindowComponent;
}

namespace ecs
{
	class EntityWorld;
	template <typename... TTypes>
	class WorldView;
}

namespace debug
{
	class ImNodesDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			debug::ImNodesDemoWindowComponent,
			const debug::ImNodesDemoRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
};