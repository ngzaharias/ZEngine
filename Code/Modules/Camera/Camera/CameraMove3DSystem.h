#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Move3DComponent;
	struct SettingsSingleton;
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
	class Move3DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			camera::Move3DComponent,
			camera::SettingsSingleton,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}