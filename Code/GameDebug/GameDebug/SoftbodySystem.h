#pragma once

#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace softbody
{
	struct ChainComponent;
}

namespace dbg
{
	class SoftbodySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const softbody::ChainComponent,
			const eng::settings::DebugComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}