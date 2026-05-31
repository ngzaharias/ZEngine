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
	struct Move2DComponent;
	struct Move2DTemplate;
	struct SettingsComponent;
}

namespace shared::camera
{
	class Move2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write <
			eng::InputManager,
			eng::TransformComponent,
			shared::camera::Move2DComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			shared::camera::Move2DTemplate,
			shared::camera::SettingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}