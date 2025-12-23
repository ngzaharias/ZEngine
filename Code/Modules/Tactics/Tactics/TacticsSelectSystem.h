#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::PhysicsSceneSingleton,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			tactics::SelectedComponent,
			tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}