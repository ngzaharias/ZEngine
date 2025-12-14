#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace hexmap
{
	struct RootComponent;
}

namespace hexmap
{
	class RootSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			hexmap::RootComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}