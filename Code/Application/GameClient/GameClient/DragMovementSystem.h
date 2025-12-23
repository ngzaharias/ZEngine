#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesSingleton;
	struct NameComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace drag
{
	struct SelectionComponent;

	class MovementSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton,
			eng::TransformComponent>
			::Read<
			drag::SelectionComponent,
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}