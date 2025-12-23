#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class ImguiManager;
	class InputManager;
}

namespace debug
{
	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::ImguiManager,
			eng::InputManager>;

		void Initialise(World& world);
		void Shutdown(World& world);
	};
}