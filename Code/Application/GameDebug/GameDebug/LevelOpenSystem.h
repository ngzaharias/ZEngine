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
	struct DirectorySingleton;
	struct LoadRequest;
}

namespace dbg::level
{
	struct OpenRequestEvent;
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
			// Events
			eng::level::LoadRequest,
			const dbg::level::OpenRequestEvent,
			// Singletons
			const eng::level::DirectorySingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}