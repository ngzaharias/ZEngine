#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct LinesComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace shared::tilemap
{
	struct AgentComponent;
	struct GridComponent;
}

namespace shared::tilemap
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::settings::DebugComponent,
			eng::TransformComponent,
			eng::WindowManager,
			shared::tilemap::AgentComponent,
			shared::tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}