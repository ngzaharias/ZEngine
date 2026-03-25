#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
		using World = ecs::WorldView
			::Write<
			movement::AccelerationComponent>
			::Read<
			projectile::ChangesComponent,
			projectile::CreateRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}