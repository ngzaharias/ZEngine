#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class InputManager;
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
			eng::TransformComponent,
			shared::cursor::ClientTransformEvent>
			::Read<
			eng::InputManager>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}