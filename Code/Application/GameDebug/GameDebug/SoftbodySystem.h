#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct LinesStaticComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct DebugStaticComponent;
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
			eng::LinesStaticComponent>
			::Read<
			eng::settings::DebugStaticComponent,
			eng::TransformComponent,
			softbody::ChainComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}