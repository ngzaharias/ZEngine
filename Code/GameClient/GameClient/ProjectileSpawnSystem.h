#pragma once

#include "ECS/System.h"

namespace projectile
{
	struct ChangesComponent;
	struct CreateRequestComponent;
	struct CreateResultComponent;
	struct SpawnComponent;
	struct TrajectoryComponent;

	/// \brief
	class SpawnSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			projectile::ChangesComponent,
			projectile::CreateResultComponent,
			projectile::SpawnComponent,
			const projectile::CreateRequestComponent,
			const projectile::TrajectoryComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessCreate(World& world);
		void ProcessDestroy(World& world);
		void ProcessLifetime(World& world, const GameTime& gameTime);
	};
}