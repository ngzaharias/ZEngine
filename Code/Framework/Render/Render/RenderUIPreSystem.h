#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class UIManager;
	class WindowManager;
}

namespace render
{
	struct UIPreComponent;
}

namespace render
{
	class UIPreSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::UIManager,
			eng::WindowManager,
			render::UIPreComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}