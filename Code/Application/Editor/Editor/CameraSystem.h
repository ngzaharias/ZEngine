#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Move3DComponent;
	struct Pan3DComponent;
	struct Zoom2DComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct ActiveComponent;
	struct CameraComponent;
	struct EditorComponent;
	struct TransformComponent;
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
		using World = ecs::WorldView
			::Write<
			camera::Move3DComponent,
			camera::Pan3DComponent,
			camera::Zoom2DComponent,
			ecs::NameComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::EditorComponent,
			eng::TransformComponent>
			::Read<
			eng::settings::DebugSingleton>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
