#pragma once

#include "ECS/System.h"

namespace projectile
{
	struct ChangesComponent;
	struct CreateRequestComponent;
}

namespace movement
{
	struct AccelerationComponent;
	struct VelocityComponent;

	/// \brief
	class AccelerationSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			movement::AccelerationComponent,
			const projectile::ChangesComponent,
			const projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}