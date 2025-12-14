#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct TransformComponent;
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
			const eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::TransformComponent,
			const eng::camera::PanningComponent,
			const eng::camera::ProjectionComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}