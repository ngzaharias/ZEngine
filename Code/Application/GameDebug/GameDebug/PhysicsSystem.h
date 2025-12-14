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
	struct PhysicsComponent;
	struct RigidDynamicComponent;
	struct RigidStaticComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace debug
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