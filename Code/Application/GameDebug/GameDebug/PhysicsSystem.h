#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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

namespace debug
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesComponent>
			::Read<
			eng::PhysicsComponent,
			eng::RigidDynamicComponent,
			eng::RigidStaticComponent,
			eng::settings::DebugComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}