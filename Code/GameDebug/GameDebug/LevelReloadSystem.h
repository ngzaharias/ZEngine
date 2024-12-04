#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct LoadedComponent;
	struct LoadRequestComponent;
}

namespace dbg::level
{
	struct ReloadRequestComponent;
}

namespace dbg::level
{
	class ReloadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequestComponent,
			const eng::level::LoadedComponent,
			const dbg::level::ReloadRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}