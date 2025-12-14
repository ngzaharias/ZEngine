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
	struct PhysicsSceneSingleton;
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

namespace tactics
{
	struct SelectedComponent;
}

namespace tilemap
{
	struct AgentComponent;
}

namespace tactics
{
	class SelectSystem final : public ecs::System
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
			const tactics::SelectedComponent,
			const tilemap::AgentComponent,
			// Singleton
			const eng::PhysicsSceneSingleton,
			const eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}