#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace camera
{
	struct Bound2DComponent;
}

namespace eng
{
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace camera
{
	class Bound2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			camera::Bound2DComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);
	};
}