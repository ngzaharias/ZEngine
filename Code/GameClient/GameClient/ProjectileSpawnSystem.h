#pragma once

#include <ECS/System.h>

namespace eng
{
	struct LinesComponent;
	struct StaticMeshComponent;
	struct TransformComponent;
}

namespace movement
{
	struct AccelerationComponent;
	struct VelocityComponent;
}

namespace projectile
{
	struct LifetimeComponent;
	struct SpawnRequestComponent;
	struct TrajectoryComponent;

	/// \brief
	class SpawnSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			movement::AccelerationComponent,
			movement::VelocityComponent,
			projectile::LifetimeComponent,
			projectile::TrajectoryComponent,
			const projectile::SpawnRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}