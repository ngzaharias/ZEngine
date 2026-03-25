#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct PhysicsSceneComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace tactics
{
	struct PawnSelectedComponent;
}

namespace tilemap
{
	struct AgentComponent;
}

namespace tactics
{
	class PawnSelectionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			tactics::PawnSelectedComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::PhysicsSceneComponent,
			eng::settings::DebugComponent,
			eng::TransformComponent,
			eng::WindowManager,
			tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}