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
	struct LayerComponent;
	struct RootComponent;
}

namespace hexmap
{
	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			hexmap::LayerComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::TransformComponent,
			const hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}