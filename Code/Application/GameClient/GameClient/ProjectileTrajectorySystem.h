#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace movement
{
	struct VelocityComponent;
}

namespace projectile
{
	struct ChangesSingleton;
	struct CreateRequestComponent;
	struct TrajectoryComponent;
}

namespace projectile
{
	/// \brief
	class TrajectorySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			projectile::TrajectoryComponent>
			::Read<
			movement::VelocityComponent,
			projectile::ChangesSingleton,
			projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}