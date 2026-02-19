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

namespace gamestate
{
	struct EditModeComponent;
}

namespace camera
{
	class ActivationSystem final : public ecs::System
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
			gamestate::EditModeComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
