#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct LinesSingleton;
	struct TransformComponent;
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
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			tilemap::AgentComponent,
			tilemap::GridComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}