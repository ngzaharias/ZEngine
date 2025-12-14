#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace eng
{
	struct TransformComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct Move3DComponent;
	struct Pan3DComponent;
	struct ProjectionComponent;
	struct Zoom2DComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace editor
{
	class CameraSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			ecs::NameComponent,
			eng::camera::EditorComponent,
			eng::camera::Move3DComponent,
			eng::camera::Pan3DComponent,
			eng::camera::ProjectionComponent,
			eng::camera::Zoom2DComponent,
			eng::TransformComponent,
			// Singletons
			const eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}
