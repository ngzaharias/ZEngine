#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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

namespace shared::camera
{
	struct Move3DComponent;
	struct Pan3DComponent;
	struct Zoom2DComponent;
}

namespace shared::gamestate
{
	struct EditModeComponent;
}

namespace shared::camera
{
	class ActivationSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::EditorComponent,
			eng::TransformComponent,
			shared::camera::Move3DComponent,
			shared::camera::Pan3DComponent,
			shared::camera::Zoom2DComponent>
			::Read<
			shared::gamestate::EditModeComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}
