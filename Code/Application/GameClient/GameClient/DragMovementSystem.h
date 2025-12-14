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
	class WindowManager;
	struct LinesSingleton;
	struct NameComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace drag
{
	struct SelectionComponent;

	class MovementSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::TransformComponent,
			const drag::SelectionComponent,
			const eng::camera::ProjectionComponent,
			// Singletons
			eng::LinesSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}