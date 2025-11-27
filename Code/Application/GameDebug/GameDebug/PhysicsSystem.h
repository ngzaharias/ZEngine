#pragma once

#include "ECS/System.h"

namespace eng
{
	struct LinesSingleton;
	struct PhysicsComponent;
	struct RigidDynamicComponent;
	struct RigidStaticComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace dbg
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			const eng::PhysicsComponent,
			const eng::RigidDynamicComponent,
			const eng::RigidStaticComponent,
			// Singletons
			eng::LinesSingleton,
			const eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}