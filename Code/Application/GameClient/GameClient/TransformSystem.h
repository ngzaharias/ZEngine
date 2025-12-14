#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	struct TransformComponent;
}

namespace projectile
{
	struct ChangesSingleton;
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
			// Components
			eng::TransformComponent,
			const projectile::CreateRequestComponent,
			const projectile::TrajectoryComponent,
			// Singletons
			const projectile::ChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProjectileRequests(World& world);
	};
}