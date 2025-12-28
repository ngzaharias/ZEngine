#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct SettingsSingleton;
	struct Zoom2DComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct CameraComponent;
	struct TransformComponent;
}

namespace camera
{
	class Zoom2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::CameraComponent,
			eng::TransformComponent>
			::Read<
			camera::SettingsSingleton,
			camera::Zoom2DComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}