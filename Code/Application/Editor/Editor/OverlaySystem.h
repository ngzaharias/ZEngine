#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"

namespace eng
{
	class WindowManager;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace editor
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			eng::camera::ProjectionComponent,
			eng::TransformComponent,
			// Singletons
			const eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}