#pragma once

#include "ECS/System.h"

namespace eng
{
	class WindowManager;
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
			// Resources
			const eng::WindowManager,
			// Components
			hexmap::LayerComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::TransformComponent,
			const hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}