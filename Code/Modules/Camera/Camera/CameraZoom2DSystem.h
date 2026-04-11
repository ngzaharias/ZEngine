#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct SettingsComponent;
	struct Zoom2DComponent;
	struct Zoom2DTemplate;
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
	class Zoom2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			camera::Zoom2DComponent,
			eng::CameraComponent,
			eng::TransformComponent>
			::Read<
			camera::SettingsComponent,
			camera::Zoom2DTemplate,
			eng::ActiveComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}