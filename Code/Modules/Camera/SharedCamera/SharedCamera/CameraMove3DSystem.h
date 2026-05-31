#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace shared::camera
{
	struct Move3DComponent;
	struct Move3DTemplate;
	struct SettingsComponent;
}

namespace shared::camera
{
	class Move3DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::TransformComponent,
			shared::camera::Move3DComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			shared::camera::Move3DTemplate,
			shared::camera::SettingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}