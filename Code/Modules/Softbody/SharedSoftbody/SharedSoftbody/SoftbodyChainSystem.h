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

namespace shared::softbody
{
	struct ChainComponent;
	struct ChainTemplate;
}

namespace shared::softbody
{
	class ChainSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::InputManager,
			eng::TransformComponent,
			shared::softbody::ChainComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager,
			shared::softbody::ChainTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}