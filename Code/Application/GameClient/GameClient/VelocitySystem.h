#pragma once

#include "ECS/System.h"

namespace projectile
{
	struct ChangesSingleton;
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
			// Components
			movement::VelocityComponent,
			const movement::AccelerationComponent,
			const projectile::CreateRequestComponent,
			// Singletons
			const projectile::ChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}