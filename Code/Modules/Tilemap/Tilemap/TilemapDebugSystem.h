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
	struct LinesSingleton;
	struct TransformComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
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
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			const tilemap::AgentComponent,
			const tilemap::GridComponent,
			// Singletons
			eng::LinesSingleton,
			const eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}