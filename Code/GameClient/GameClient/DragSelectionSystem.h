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
	struct LinesComponent;
	struct PhysicsSceneComponent;
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
			eng::LinesComponent,
			const drag::IsSelectableComponent,
			const eng::camera::ProjectionComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}