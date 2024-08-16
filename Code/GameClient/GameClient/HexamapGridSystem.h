#pragma once

#include "ECS/System.h"

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct LinesComponent;
	struct TransformComponent;
}

namespace hexamap
{
	class GridSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const eng::CameraComponent,
			const eng::InputComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}