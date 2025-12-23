#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class WindowManager;
	struct TransformComponent;
}

namespace eng::camera
{
	struct Bound2DComponent;
	struct ProjectionComponent;

	class Bound2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			eng::camera::Bound2DComponent,
			eng::camera::ProjectionComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}