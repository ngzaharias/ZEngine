#pragma once

#include "ECS/System.h"

namespace ecs
{
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

namespace eng::level
{
	struct LoadedComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace editor
{
	class CameraSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::camera::EditorComponent,
			eng::camera::Move3DComponent,
			eng::camera::Pan3DComponent,
			eng::camera::ProjectionComponent,
			eng::camera::Zoom2DComponent,
			eng::TransformComponent,
			const eng::level::LoadedComponent,
			const eng::settings::DebugComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
