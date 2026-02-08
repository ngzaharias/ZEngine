#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Move2DComponent;
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

	class Move2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::TransformComponent>
			::Read<
			camera::Move2DComponent,
			camera::SettingsSingleton,
			eng::ActiveComponent,
			eng::CameraComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}