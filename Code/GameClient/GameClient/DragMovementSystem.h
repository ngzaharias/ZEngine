#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
	struct LinesComponent;
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
			eng::LinesComponent,
			eng::TransformComponent,
			const drag::SelectionComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}