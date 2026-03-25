#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct LinesComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct DebugComponent;
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
			eng::LinesComponent>
			::Read<
			eng::settings::DebugComponent,
			eng::TransformComponent,
			softbody::ChainComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}