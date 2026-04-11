#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Move2DComponent;
	struct Move2DTemplate;
	struct SettingsComponent;
}

namespace eng
{
	class InputManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace camera
{

	class Move2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			camera::Move2DComponent,
			eng::InputManager,
			eng::TransformComponent>
			::Read<
			camera::Move2DTemplate,
			camera::SettingsComponent,
			eng::ActiveComponent,
			eng::CameraComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}