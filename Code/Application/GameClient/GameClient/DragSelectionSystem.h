#pragma once

#include "ECS/System.h"

namespace drag
{
	struct IsSelectableComponent;
	struct SelectionComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesSingleton;
	struct PhysicsSceneSingleton;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace drag
{
	class SelectionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			drag::SelectionComponent,
			ecs::NameComponent,
			const drag::IsSelectableComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			// Singletons
			eng::LinesSingleton,
			const eng::PhysicsSceneSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}