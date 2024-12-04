#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace hexmap
{
	struct RootComponent;
}

namespace hexmap
{
	class RootSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			hexmap::RootComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}