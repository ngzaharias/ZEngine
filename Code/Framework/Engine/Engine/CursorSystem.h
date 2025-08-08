#pragma once

#include "ECS/System.h"

namespace eng
{
	class InputManager;
	struct CursorComponent;
	struct SpriteComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng
{
	class CursorSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::camera::ProjectionComponent,
			eng::CursorComponent,
			eng::InputManager,
			eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
};