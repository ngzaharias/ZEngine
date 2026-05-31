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

namespace shared::hexmap
{
	struct RootComponent;
	struct RootTemplate;
}

namespace shared::hexmap
{
	class RootSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::CameraComponent,
			eng::InputManager,
			shared::hexmap::RootComponent>
			::Read<
			eng::ActiveComponent,
			eng::TransformComponent,
			eng::WindowManager,
			shared::hexmap::RootTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}