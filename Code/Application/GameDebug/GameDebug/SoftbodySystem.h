#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	struct LinesSingleton;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace softbody
{
	struct ChainComponent;
}

namespace debug
{
	class SoftbodySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			const softbody::ChainComponent,
			// Singletons
			eng::LinesSingleton,
			const eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}