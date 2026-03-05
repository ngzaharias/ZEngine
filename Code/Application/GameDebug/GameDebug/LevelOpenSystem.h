#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
	struct LoadRequestEvent;
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
		using World = ecs::WorldView
			::Write<
			debug::level::OpenWindowComponent,
			ecs::NameComponent,
			eng::InputManager,
			eng::level::DirectorySingleton,
			eng::level::LoadRequestEvent>
			::Read<
			debug::level::OpenRequest>;

		void Update(World& world, const GameTime& gameTime);
	};
}