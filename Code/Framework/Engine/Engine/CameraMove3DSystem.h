#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	struct TransformComponent;
}

namespace eng::settings
{
	struct CameraSingleton;
}

namespace eng::camera
{
	struct Move3DComponent;
	struct ProjectionComponent;

	class Move3DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			eng::camera::Move3DComponent,
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::settings::CameraSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}