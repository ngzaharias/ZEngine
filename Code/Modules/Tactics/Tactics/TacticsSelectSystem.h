#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct PhysicsSceneSingleton;
	struct TransformComponent;
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
			::Write<
			tactics::SelectedComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::PhysicsSceneSingleton,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}