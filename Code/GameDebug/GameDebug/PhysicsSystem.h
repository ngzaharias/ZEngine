#pragma once

#include "ECS/System.h"

namespace eng
{
	struct LinesComponent;
	struct PhysicsComponent;
	struct RigidDynamicComponent;
	struct RigidStaticComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace dbg
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			const eng::PhysicsComponent,
			const eng::RigidDynamicComponent,
			const eng::RigidStaticComponent,
			const eng::settings::DebugComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}