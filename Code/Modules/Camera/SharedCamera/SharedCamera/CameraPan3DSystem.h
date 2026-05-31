#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace shared::camera
{
	struct Pan3DComponent;
	struct Pan3DTemplate;
}

namespace shared::camera
{
	class Pan3DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::TransformComponent,
			shared::camera::Pan3DComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager,
			shared::camera::Pan3DTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}