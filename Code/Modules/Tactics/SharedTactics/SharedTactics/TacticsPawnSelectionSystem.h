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

namespace shared::tactics
{
	struct PawnSelectedComponent;
}

namespace shared::tilemap
{
	struct AgentComponent;
}

namespace shared::tactics
{
	class PawnSelectionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			shared::tactics::PawnSelectedComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::PhysicsSceneComponent,
			eng::settings::DebugComponent,
			eng::TransformComponent,
			eng::WindowManager,
			shared::tilemap::AgentComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}