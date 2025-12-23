#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			projectile::ChangesSingleton,
			projectile::CreateResultComponent,
			projectile::SpawnComponent>
			::Read<
			projectile::CreateRequestComponent,
			projectile::TrajectoryComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessCreate(World& world);
		void ProcessDestroy(World& world);
		void ProcessLifetime(World& world, const GameTime& gameTime);
	};
}