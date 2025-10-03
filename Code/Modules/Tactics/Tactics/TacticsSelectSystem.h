#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct PhysicsSceneComponent;
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
			const eng::settings::DebugComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent,
			const tactics::SelectedComponent,
			const tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}