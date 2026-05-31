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
	struct LayerComponent;
	struct RootComponent;
}

namespace shared::hexmap
{
	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			shared::hexmap::LayerComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::TransformComponent,
			eng::WindowManager,
			shared::hexmap::RootComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}