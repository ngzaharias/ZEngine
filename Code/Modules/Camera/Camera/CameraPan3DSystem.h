#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Pan3DComponent;
	struct Pan3DTemplate;
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
			camera::Pan3DComponent,
			eng::InputManager,
			eng::TransformComponent>
			::Read<
			camera::Pan3DTemplate,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}