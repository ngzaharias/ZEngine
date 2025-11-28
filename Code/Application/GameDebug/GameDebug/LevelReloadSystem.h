#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct LoadedComponent;
	struct LoadRequestEvent;
}

namespace dbg::level
{
	struct ReloadRequest;
}

namespace dbg::level
{
	class ReloadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequestEvent,
			const eng::level::LoadedComponent,
			const dbg::level::ReloadRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}