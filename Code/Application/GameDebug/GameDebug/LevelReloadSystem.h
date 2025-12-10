#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct LoadedComponent;
	struct LoadRequest;
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
		using World = ecs::WorldView<
			eng::level::LoadRequest,
			const eng::level::LoadedComponent,
			const debug::level::ReloadRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}