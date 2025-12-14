#pragma once

#include "ECS/System.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

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
	class AccelerationSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			movement::AccelerationComponent,
			const projectile::CreateRequestComponent,
			// Singletons
			const projectile::ChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);
	};
}