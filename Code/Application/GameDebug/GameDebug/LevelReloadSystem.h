#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::level
{
	struct LoadedComponent;
	struct LoadEvent;
}

namespace debug::level
{
	struct ReloadEvent;
}

namespace debug::level
{
	class ReloadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::level::LoadEvent>
			::Read<
			debug::level::ReloadEvent,
			eng::level::LoadedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}