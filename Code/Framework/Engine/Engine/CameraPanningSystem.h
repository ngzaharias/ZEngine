#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			eng::camera::PanningComponent,
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}