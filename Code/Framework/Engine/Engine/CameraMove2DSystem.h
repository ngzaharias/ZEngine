#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
	struct TransformComponent;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace eng::camera
{
	struct Move2DComponent;

	class Move2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			// Components
			eng::TransformComponent,
			const eng::camera::Move2DComponent,
			const eng::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}