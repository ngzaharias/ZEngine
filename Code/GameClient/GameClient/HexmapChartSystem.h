#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace hexmap
{
	struct ChartComponent;
	struct RequestComponent;
	struct SettingsComponent;
}

namespace hexmap
{
	class ChartSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			hexmap::ChartComponent,
			hexmap::RequestComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::TransformComponent,
			const hexmap::SettingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}