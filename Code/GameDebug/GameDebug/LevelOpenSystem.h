#pragma once

#include "ECS/System.h"

namespace eng::level
{
	struct DirectoryComponent;
	struct LoadRequestComponent;
}

namespace dbg::level
{
	struct OpenRequestComponent;
}

namespace dbg::level
{
	class OpenSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::level::LoadRequestComponent,
			const dbg::level::OpenRequestComponent,
			const eng::level::DirectoryComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}