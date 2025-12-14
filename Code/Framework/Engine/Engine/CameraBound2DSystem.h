#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class WindowManager;
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
			const eng::camera::ProjectionComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}