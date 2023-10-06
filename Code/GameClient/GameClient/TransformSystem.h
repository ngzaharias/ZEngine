#pragma once

#include <ECS/System.h>

namespace eng
{
	struct TransformComponent;
}

namespace projectile
{
	struct ChangesComponent;
	struct CreateRequestComponent;
	struct TrajectoryComponent;
}

namespace transform
{
	/// \brief
	class TransformSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::TransformComponent,
			const projectile::ChangesComponent,
			const projectile::CreateRequestComponent,
			const projectile::TrajectoryComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProjectileRequests(World& world);
	};
}