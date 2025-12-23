#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			eng::TransformComponent>
			::Read<
			projectile::ChangesSingleton,
			projectile::CreateRequestComponent,
			projectile::TrajectoryComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProjectileRequests(World& world);
	};
}