#pragma once

#include "ECS/System.h"

namespace eng
{
	class Window;
}

namespace eng::settings
{
	struct GraphicsComponent;
}

namespace eng
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::Window,
			// Components
			const eng::settings::GraphicsComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}