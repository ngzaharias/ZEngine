#pragma once

#include "ECS/System.h"

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
		using World = ecs::WorldView<
			// Components
			projectile::TrajectoryComponent,
			const movement::VelocityComponent,
			const projectile::CreateRequestComponent,
			// Singletons
			const projectile::ChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}