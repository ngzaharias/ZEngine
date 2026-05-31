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
	struct SettingsComponent;
	struct Zoom2DComponent;
	struct Zoom2DTemplate;
}

namespace shared::camera
{
	class Zoom2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::CameraComponent,
			eng::TransformComponent,
			shared::camera::Zoom2DComponent>
			::Read<
			eng::ActiveComponent,
			eng::InputManager,
			eng::WindowManager,
			shared::camera::SettingsComponent,
			shared::camera::Zoom2DTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}