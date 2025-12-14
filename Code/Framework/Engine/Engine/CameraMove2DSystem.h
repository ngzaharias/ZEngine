#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

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
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			// Components
			eng::TransformComponent,
			const eng::camera::Move2DComponent,
			// Singletons
			const eng::settings::CameraSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}