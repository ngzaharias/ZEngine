#pragma once

#include <ECS/System.h>

namespace movement
{
	struct VelocityComponent;
}

namespace projectile
{
	struct ChangesComponent;
	struct CreateRequestComponent;
	struct TrajectoryComponent;

	/// \brief
	class TrajectorySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			projectile::TrajectoryComponent,
			const movement::VelocityComponent,
			const projectile::ChangesComponent,
			const projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}