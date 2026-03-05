#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::level
{
	struct LoadedComponent;
	struct LoadRequestEvent;
}

namespace debug::level
{
	struct ReloadRequest;
}

namespace debug::level
{
	class ReloadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::level::LoadRequestEvent>
			::Read<
			debug::level::ReloadRequest,
			eng::level::LoadedComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}