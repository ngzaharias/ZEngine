#pragma once

#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class InputManager;
}

namespace eng::level
{
	struct DirectoryComponent;
	struct LoadRequestComponent;
}

namespace dbg::level
{
	struct OpenRequestComponent;
	struct WindowComponent;
}

namespace dbg::level
{
	class OpenSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			// Components
			dbg::level::WindowComponent,
			ecs::NameComponent,
			eng::level::LoadRequestComponent,
			const dbg::level::OpenRequestComponent,
			const eng::level::DirectoryComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}