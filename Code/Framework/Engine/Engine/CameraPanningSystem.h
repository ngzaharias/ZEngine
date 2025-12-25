#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct TransformComponent;
}

namespace eng::camera
{
	struct Pan3DComponent;
	struct ProjectionComponent;
}

namespace eng::camera
{
	class PanningSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			eng::camera::Pan3DComponent,
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}