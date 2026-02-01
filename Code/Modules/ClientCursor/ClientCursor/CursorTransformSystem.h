#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class InputManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct SpriteComponent;
	struct TransformComponent;
}

namespace shared::cursor
{
	struct ClientTransformEvent;
}

namespace client::cursor
{
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			shared::cursor::ClientTransformEvent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::InputManager,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}