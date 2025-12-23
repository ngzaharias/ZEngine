#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct TransformComponent;
}

namespace eng::settings
{
	struct CameraSingleton;
}

namespace eng::camera
{
	struct ProjectionComponent;
	struct Zoom2DComponent;

	class Zoom2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::camera::ProjectionComponent,
			eng::TransformComponent>
			::Read<
			eng::camera::Zoom2DComponent,
			eng::InputManager,
			eng::settings::CameraSingleton,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}