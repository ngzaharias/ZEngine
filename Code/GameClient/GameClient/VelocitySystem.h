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
	class VelocitySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			movement::VelocityComponent,
			const movement::AccelerationComponent,
			const projectile::ChangesComponent,
			const projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}