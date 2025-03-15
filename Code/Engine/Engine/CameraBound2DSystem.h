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
	struct Bound2DComponent;
	struct ProjectionComponent;

	class Bound2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			eng::TransformComponent,
			const eng::camera::Bound2DComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}