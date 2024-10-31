#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
	struct LinesComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace hexamap
{
	class GridSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}