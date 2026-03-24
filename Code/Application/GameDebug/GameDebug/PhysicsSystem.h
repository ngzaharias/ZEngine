#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct LinesStaticComponent;
	struct PhysicsComponent;
	struct RigidDynamicComponent;
	struct RigidStaticComponent;
}

namespace eng::settings
{
	struct DebugStaticComponent;
}

namespace debug
{
	class PhysicsSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesStaticComponent>
			::Read<
			eng::PhysicsComponent,
			eng::RigidDynamicComponent,
			eng::RigidStaticComponent,
			eng::settings::DebugStaticComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}