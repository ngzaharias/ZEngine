#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

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
		using World = ecs::WorldView<
			eng::ImguiManager,
			eng::InputManager>;

		void Initialise(World& world);
		void Shutdown(World& world);
	};
}