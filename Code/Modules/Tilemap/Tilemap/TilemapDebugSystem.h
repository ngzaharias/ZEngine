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
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace tilemap
{
	struct AgentComponent;
	struct GridComponent;
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
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::settings::DebugComponent,
			const eng::TransformComponent,
			const tilemap::AgentComponent,
			const tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}