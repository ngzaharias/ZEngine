#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace tilemap
{
	struct AgentComponent;
}

namespace tilemap
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::LinesComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			const tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}