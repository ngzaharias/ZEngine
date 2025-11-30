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
	struct OpenRequest;
	struct OpenWindowComponent;
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
			dbg::level::OpenWindowComponent,
			ecs::NameComponent,
			// Events
			eng::level::LoadRequest,
			const dbg::level::OpenRequest,
			// Singletons
			const eng::level::DirectorySingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}