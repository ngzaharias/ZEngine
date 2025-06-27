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
	struct Move3DComponent;
	struct ProjectionComponent;

	class Move3DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			// Components
			eng::TransformComponent,
			const eng::camera::Move3DComponent,
			const eng::camera::ProjectionComponent,
			const eng::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}