#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct ImNodesDemoEvent;
	struct ImNodesDemoWindowComponent;
}

namespace ecs
{
	class EntityWorld;
}

namespace debug
{
	class ImNodesDemoSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::ImNodesDemoWindowComponent>
			::Read<
			debug::ImNodesDemoEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
};