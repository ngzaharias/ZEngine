#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class WindowManager;
	struct FileDropEvent;
}

namespace eng
{
	class FileDropSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::FileDropEvent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
};