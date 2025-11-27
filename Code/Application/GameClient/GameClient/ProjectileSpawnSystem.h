#pragma once

#include "ECS/System.h"

namespace projectile
{
	struct ChangesSingleton;
	struct CreateRequestComponent;
	struct CreateResultComponent;
	struct SpawnComponent;
	struct TrajectoryComponent;
}

namespace projectile
{
	/// \brief
	class SpawnSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			projectile::CreateResultComponent,
			projectile::SpawnComponent,
			const projectile::CreateRequestComponent,
			const projectile::TrajectoryComponent,
			// Singletons
			projectile::ChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessCreate(World& world);
		void ProcessDestroy(World& world);
		void ProcessLifetime(World& world, const GameTime& gameTime);
	};
}