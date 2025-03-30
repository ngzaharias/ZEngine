#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ECS/System.h"

namespace eng
{
	class InputManager;
	class WindowManager;
	struct LinesComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
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
		using World = ecs::WorldView<
			// Resources
			eng::InputManager,
			const eng::WindowManager,
			// Components
			eng::LinesComponent,
			eng::TransformComponent,
			softbody::ChainComponent,
			const eng::camera::ProjectionComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}