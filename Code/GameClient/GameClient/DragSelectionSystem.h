#pragma once

#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct InputComponent;
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
	struct IsSelectableComponent;
	struct SelectionComponent;

	class SelectionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			drag::SelectionComponent,
			ecs::NameComponent,
			eng::LinesComponent,
			const drag::IsSelectableComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}