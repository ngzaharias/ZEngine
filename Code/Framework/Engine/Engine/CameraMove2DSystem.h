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
	struct Move2DComponent;

	class Move2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::TransformComponent>
			::Read<
			eng::camera::Move2DComponent,
			eng::settings::CameraSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}