#pragma once

#include "ECS/System.h"

namespace eng
{
	class WindowManager;
	struct InputComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
	struct Zoom2DComponent;

	class Zoom2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			eng::camera::ProjectionComponent,
			eng::TransformComponent,
			const eng::camera::Zoom2DComponent,
			const eng::InputComponent,
			const eng::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}