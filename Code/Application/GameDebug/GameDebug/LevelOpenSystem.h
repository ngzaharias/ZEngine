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

namespace debug::level
{
	struct OpenRequest;
	struct OpenWindowComponent;
}

namespace debug::level
{
	class OpenSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			// Components
			debug::level::OpenWindowComponent,
			ecs::NameComponent,
			// Events
			eng::level::LoadRequest,
			const debug::level::OpenRequest,
			// Singletons
			const eng::level::DirectorySingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}