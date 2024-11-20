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
	struct FragmentComponent;
	struct SettingsComponent;
}

namespace hexmap
{
	class ModifySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			hexmap::FragmentComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent,
			const eng::TransformComponent,
			const hexmap::ChartComponent,
			const hexmap::SettingsComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}