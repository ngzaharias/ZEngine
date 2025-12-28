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

namespace softbody
{
	struct ChainComponent;
}

namespace softbody
{
	class ChainSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::TransformComponent,
			softbody::ChainComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}