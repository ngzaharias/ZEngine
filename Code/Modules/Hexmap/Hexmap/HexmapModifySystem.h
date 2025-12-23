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
	struct ProjectionComponent;
}

namespace hexmap
{
	struct LayerComponent;
	struct RootComponent;
}

namespace hexmap
{
	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			hexmap::LayerComponent>
			::Read<
			eng::camera::ProjectionComponent,
			eng::InputManager,
			eng::TransformComponent,
			eng::WindowManager,
			hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}