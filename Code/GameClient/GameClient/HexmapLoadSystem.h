#pragma once

#include "ECS/System.h"

namespace clt::settings
{
	struct DebugComponent;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct LinesComponent;
	struct TransformComponent;
}

namespace hexmap
{
	struct ChartComponent;
	struct FragmentComponent;
	struct RequestComponent;
}

namespace hexmap
{
	class LoadSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::LinesComponent,
			eng::TransformComponent,
			hexmap::FragmentComponent,
			const clt::settings::DebugComponent,
			const hexmap::ChartComponent,
			const hexmap::RequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}