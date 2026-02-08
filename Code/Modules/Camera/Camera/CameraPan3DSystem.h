#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Pan3DComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace camera
{
	class Pan3DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			camera::Pan3DComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}