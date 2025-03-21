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
	struct PanningComponent;
	struct ProjectionComponent;

	class PanningSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			eng::TransformComponent,
			const eng::camera::PanningComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}