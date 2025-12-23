#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton>
			::Read<
			eng::settings::DebugSingleton,
			softbody::ChainComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}