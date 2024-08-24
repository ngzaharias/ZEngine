#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace eng::camera
{
	struct BehaviourComponent;
	struct ProjectionComponent;

	class Zoom2DSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::camera::ProjectionComponent,
			const eng::camera::BehaviourComponent,
			const eng::InputComponent,
			const eng::settings::LocalComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}