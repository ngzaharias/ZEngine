#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace hexmap
{
	struct RootComponent;
}

namespace hexmap
{
	class RootSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::CameraComponent,
			eng::InputManager,
			hexmap::RootComponent>
			::Read<
			eng::ActiveComponent,
			eng::TransformComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}