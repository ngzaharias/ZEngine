#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class InputManager;
	class WindowManager;
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
			eng::TransformComponent,
			softbody::ChainComponent,
			const eng::camera::ProjectionComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}