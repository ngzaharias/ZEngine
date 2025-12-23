#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton>
			::Read<
			eng::PhysicsComponent,
			eng::RigidDynamicComponent,
			eng::RigidStaticComponent,
			eng::settings::DebugSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}