#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class WindowManager;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace editor
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::camera::ProjectionComponent,
			eng::TransformComponent>
			::Read<
			eng::settings::DebugSingleton,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}