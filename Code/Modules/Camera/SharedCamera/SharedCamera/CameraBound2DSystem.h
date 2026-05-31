#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace shared::camera
{
	struct Bound2DComponent;
	struct Bound2DTemplate;
}

namespace shared::camera
{
	class Bound2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent,
			shared::camera::Bound2DComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::WindowManager,
			shared::camera::Bound2DTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}