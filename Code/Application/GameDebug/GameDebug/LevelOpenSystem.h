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
	struct DirectoryStaticComponent;
	struct LoadEvent;
}

namespace debug::level
{
	struct OpenEvent;
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
			eng::level::DirectoryStaticComponent,
			eng::level::LoadEvent>
			::Read<
			debug::level::OpenEvent>;

		void Update(World& world, const GameTime& gameTime);
	};
}